overload_extend

f = overload_extend.Foo();
if (f.test() != 0)
    error
endif
if (f.test(3) != 1)
    error
endif
if (f.test("hello") != 2)
    error
endif
if (f.test(3,2) != 5)
    error
endif
if (f.test(3.1)-.1 != 1003) # :)
    error
endif

