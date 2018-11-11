# umptr
UmPointer

UmPointer is a smart pointer. You can decide whether actual data, which is holding by umpointer, will be erased or not when umpointer has destroyed.

Even if umpointer's actual data has erased, you can still request actual data without any danger. umpointer will return null pointer in this case.

You can erase the actual data before umpointer destroyed.

You can register any umpointer to see which other umpointer has destroying the actual data that holded by registered umpointer.

UmPointer is non-copyable class. You can use "copyFrom" function to get a copy of an umpointer. You cannot use directly in vector (std::vector<umptr>). If you want to use in a vector you can use with pointer.(std::vector<umptr*>)

UmPointer has blank constructor.

You can use constructor overload or "init" function to initialize umpointer. When you initialized with one of these options umpointer will
bind to its actual data. This means when umpointer destroyed, the actual data will be destroyed too. You can change binding option with "Bound"
and "UnBound" functions. You can still reach the actual data after "UnBound"ing.

"getData" and "eraseData" functions have overloads that is taking line and file as arguments. If a problem occurs you can reach those information
in the errorlog (errorlog is not implemented yet. It is just printing to console.). These errors is not going to corrupt memory.

All necessary system is in the umContainer header and source file. You are just going to use umptr class. Other classes in there are using by umptr.
