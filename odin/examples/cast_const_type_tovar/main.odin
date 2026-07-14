package cast_const

import "core:fmt"

main :: proc() {

    MYCONST: f32 : 5.55
    fmt.println(MYCONST)
    fmt.println(typeid_of(type_of(MYCONST)))

    myvar: f64 = f64(MYCONST) // casting
    fmt.println(myvar)
    fmt.println(typeid_of(type_of(myvar)))

}