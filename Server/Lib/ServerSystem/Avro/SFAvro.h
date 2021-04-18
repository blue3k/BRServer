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


#include "SFTypedefs.h"




typedef struct avro_obj_t* avro_schema_t;
typedef struct avro_value_iface  avro_value_iface_t; 
typedef struct avro_value avro_value_t;
typedef struct avro_reader_t_* avro_reader_t;
typedef struct avro_writer_t_* avro_writer_t;


namespace SF
{
	namespace Avro
	{
		// The definition should be same with avro_type_t
		enum ValueType : uint8_t
		{
			String,
			Bytes,
			Int32,
			Int64,
			Float,
			Double,
			Boolean,
			Null,
			Record,
			Enum,
			Fixed,
			Map,
			Array,
			Union,
			Link
		};

		const char* GetErrorString();
	}



	class AvroSchema
	{
	public:
		AvroSchema()
		{
		}

		AvroSchema(const avro_schema_t& schema)
			: m_Handle(schema)
		{
		}

		AvroSchema(const SF::Array<char>& schemaData);

		~AvroSchema();

		operator avro_schema_t() { return m_Handle; }
		operator avro_schema_t() const { return m_Handle; }

	private:
		avro_schema_t m_Handle{};
	};



	class AvroValue
	{
	public:

		AvroValue() = default;

		AvroValue(const AvroSchema& schema);

		~AvroValue();

		operator avro_value_t* () { return &m_DataValue; }
		operator const avro_value_t* () const { return &m_DataValue; }


		Avro::ValueType GetType() const;

		Result GetField(const char* fieldName, AvroValue& value) const;

		// Accessor for the value
		template<class FieldDataType>
		FieldDataType As()
		{
			assert(false);//, "Not implemented for this type";
			return FieldDataType{};
		}

		const char* AsString();
		int AsInt();
		float AsFloat();
		double AsDouble();
		bool AsBoolean();
		ArrayView<const uint8_t> AsBytes();


		// for array/map
		size_t GetElementCount();

		// for array/map
		Result GetElement(int i, AvroValue& value);


		template<class FieldDataType>
		FieldDataType GetFieldValue(const char* fieldName)
		{
			SF::AvroValue fieldValue;
			Result res = GetField(fieldName, fieldValue);
			if (!res)
			{
				SFLog(System, Error, "Invalid avro field request. field:{0}", fieldName);
				return FieldDataType{};
			}

			return fieldValue.As<FieldDataType>();
		}


	private:
		bool m_OwnerOfValue = false;

		uint8_t m_ValueBuffer[2 * sizeof(intptr_t)]{};
		avro_value_t& m_DataValue = *(avro_value_t*)m_ValueBuffer;

		avro_value_iface_t* m_DataClass{};
	};

	template<> inline const char* AvroValue::As() { return AsString(); }
	template<> inline int AvroValue::As() { return AsInt(); }
	template<> inline float AvroValue::As() { return AsFloat(); }
	template<> inline double AvroValue::As() { return AsDouble(); }
	template<> inline bool AvroValue::As() { return AsBoolean(); }



	class AvroReader
	{
	public:
		AvroReader()
		{
		}

		AvroReader(const avro_reader_t& reader)
			: m_Handle(reader)
		{
		}

		AvroReader(SF::Array<char>& binData);

		~AvroReader();

		operator avro_reader_t() { return m_Handle; }
		operator avro_reader_t() const { return m_Handle; }

		Result ReadValue(AvroValue& value);

	private:
		avro_reader_t m_Handle{};
	};

}
