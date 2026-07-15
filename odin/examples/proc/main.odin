package procs

import "core:fmt"

print_hello :: proc() {
    fmt.println("Hello")
}

main :: proc() {
	
    print_hello()

}