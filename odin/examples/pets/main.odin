package test_program

import "core:fmt"

main :: proc() {

    treats := 5
    fav := 3

    pets := [8]int {
        1,2,3,4,5,6,7,8
    }

    for t := 0; t <= treats; t += 1 {

        for i := 0; i < 8; i += 1 {
            fmt.printfln("Pet #%v has gotten a treat", pets[i])
            if i == fav {
                most_loved(i)
            }
        }
    }


most_loved :: proc(pet: int) {
    fmt.printfln("Pet %v is the most favorite", pet)
}

}