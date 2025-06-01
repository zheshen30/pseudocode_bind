# pseudocode_bind
this is pseudocode for stl bind

The pseudo-code actually imitate and extends the standard template library, but there are some function don't realized in this pseudo-code. 
In the standard library, if you want to bind a class's non-static member function, the second parameter must be an object pointer or reference type (std::ref()). However, in this case, you can directly pass the value type of the object without the need for ref wrapping.  However, such an approach is not allowed in the standard library. Therefore, in the standard library, the second parameter cannot be a temporary object, but in this pseudo-code, it can be.

something can be improved:
In this pseudocode, the argunment cannot be a smart pointer, but in the standard library, the pointer can be a smart pointer. This is something can be improved
