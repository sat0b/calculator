#!/bin/bash

function assert-equal() {
    argument="$1"
    expect="$2"
    actual=$(./calculator "$argument")
    if [ ! "$actual" = "$expect" ]; then
        echo "$argument, $expect expected, but got $actual"
        exit 1
    fi
}

assert-equal test/ex1.ca 30
assert-equal test/ex2.ca 1300
assert-equal test/ex3.ca 245
assert-equal test/ex4.ca 0
assert-equal test/ex5.ca 1
assert-equal test/ex6.ca 2
assert-equal test/ex7.ca 1
assert-equal test/ex8.ca 2

echo OK
