////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Avro wrapper
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "ServerSystemPCH.h"
#include "Avro/SFAvro.h"

#include "avro.h"


namespace SF
{
	namespace Avro
	{

		static_assert(ValueType::String == ValueType(avro_type_t::AVRO_STRING), "The definition should be same with avro_type_t");
		static_assert(ValueType::Bytes == ValueType(avro_type_t::AVRO_BYTES), "The definition should be same with avro_type_t");
		static_assert(ValueType::Int32 == ValueType(avro_type_t::AVRO_INT32), "The definition should be same with avro_type_t");
		static_assert(ValueType::Int64 == ValueType(avro_type_t::AVRO_INT64), "The definition should be same with avro_type_t");
		static_assert(ValueType::Float == ValueType(avro_type_t::AVRO_FLOAT), "The definition should be same with avro_type_t");
		static_assert(ValueType::Double == ValueType(avro_type_t::AVRO_DOUBLE), "The definition should be same with avro_type_t");
		static_assert(ValueType::Boolean == ValueType(avro_type_t::AVRO_BOOLEAN), "The definition should be same with avro_type_t");
		static_assert(ValueType::Null == ValueType(avro_type_t::AVRO_NULL), "The definition should be same with avro_type_t");
		static_assert(ValueType::Record == ValueType(avro_type_t::AVRO_RECORD), "The definition should be same with avro_type_t");
		static_assert(ValueType::Enum == ValueType(avro_type_t::AVRO_ENUM), "The definition should be same with avro_type_t");
		static_assert(ValueType::Fixed == ValueType(avro_type_t::AVRO_FIXED), "The definition should be same with avro_type_t");
		static_assert(ValueType::Map == ValueType(avro_type_t::AVRO_MAP), "The definition should be same with avro_type_t");
		static_assert(ValueType::Array == ValueType(avro_type_t::AVRO_ARRAY), "The definition should be same with avro_type_t");
		static_assert(ValueType::Union == ValueType(avro_type_t::AVRO_UNION), "The definition should be same with avro_type_t");
		static_assert(ValueType::Link == ValueType(avro_type_t::AVRO_LINK), "The definition should be same with avro_type_t");


		const char* GetErrorString()
		{
			return avro_strerror();
		}
	}


	AvroSchema::AvroSchema(const avro_schema_t& schema)
		: m_Handle(schema)
	{
	}

	AvroSchema::AvroSchema(const AvroSchema& schema)
		: m_Handle(schema.m_Handle)
	{
		if (m_Handle != nullptr)
			avro_schema_incref(m_Handle);
	}
	
	AvroSchema::AvroSchema(const Array<char>& schemaData)
	{
		Init(schemaData);
	}

	AvroSchema::~AvroSchema()
	{
		if (m_Handle != nullptr)
			avro_schema_decref(m_Handle);
	}

	Result AvroSchema::Init(const Array<char>& schemaData)
	{
		int res = avro_schema_from_json_length(schemaData.data(), schemaData.size(), &m_Handle);
		if (res)
		{
			SFLog(System, Error, "Error parsing schema file: {0}", avro_strerror());
			return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}



	AvroValue::AvroValue(const AvroSchema& schema)
	{
		m_OwnerOfValue = true;
		m_DataClass = avro_generic_class_from_schema(schema);
		avro_generic_value_new(m_DataClass, &m_DataValue);
	}

	AvroValue::~AvroValue()
	{
		if (m_OwnerOfValue)
			avro_value_decref(&m_DataValue);

		if (m_DataClass != nullptr)
		{
			avro_value_iface_decref(m_DataClass);
		}
	}


	Avro::ValueType AvroValue::GetType() const
	{
		return (Avro::ValueType)avro_value_get_type(&m_DataValue);
	}

	Result AvroValue::GetField(const char* fieldName, AvroValue& value) const
	{
		int res = avro_value_get_by_name(&m_DataValue, fieldName, value, NULL);
		if (res != 0)
		{
			return ResultCode::NOT_EXIST;
		}

		return ResultCode::SUCCESS;
	}

	const char* AvroValue::AsString()
	{
		const char* stringValue{};
		size_t strLen = 0;
		int res = avro_value_get_string(&m_DataValue, &stringValue, &strLen);
		if (res != 0)
			return nullptr;

		return stringValue;
	}

	int AvroValue::AsInt()
	{
		int value{};
		int res = avro_value_get_int(&m_DataValue, &value);
		if (res != 0)
			return 0;

		return value;
	}

	float AvroValue::AsFloat()
	{
		float value{};
		int res = avro_value_get_float(&m_DataValue, &value);
		if (res != 0)
			return 0;

		return value;
	}

	double AvroValue::AsDouble()
	{
		double value{};
		int res = avro_value_get_double(&m_DataValue, &value);
		if (res != 0)
			return 0;

		return value;
	}

	bool AvroValue::AsBoolean()
	{
		int value{};
		int res = avro_value_get_boolean(&m_DataValue, &value);
		if (res != 0)
			return 0;

		return value != 0;
	}

	ArrayView<const uint8_t> AvroValue::AsBytes()
	{
		const void* buf{};
		size_t size{};
		int res = avro_value_get_bytes(&m_DataValue, &buf, &size);
		if (res != 0)
			return ArrayView<const uint8_t>();

		return ArrayView<const uint8_t>(size, reinterpret_cast<const uint8_t*>(buf));
	}

	// for array/map
	size_t AvroValue::GetElementCount()
	{
		size_t value{};
		int res = avro_value_get_size(&m_DataValue, &value);
		if (res != 0)
			return 0;

		return value;
	}

	// for array/map
	Result AvroValue::GetElement(int i, AvroValue& value)
	{
		const char* name{};
		int res = avro_value_get_by_index(&m_DataValue, i, value, &name);
		if (res != 0)
			return ResultCode::NOT_EXIST;

		return ResultCode::SUCCESS;
	}




	AvroReader::AvroReader(SF::Array<char>& binData)
	{
		m_Handle = avro_reader_memory(binData.data(), binData.size());
	}

	AvroReader::~AvroReader()
	{
		if (m_Handle != nullptr)
			avro_reader_free(m_Handle);
	}

	Result AvroReader::ReadValue(AvroValue& value)
	{
		int res = avro_value_read(m_Handle, value);
		if (res)
		{
			SFLog(System, Error, "Error reading value: {0}", avro_strerror());
			return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}

}
