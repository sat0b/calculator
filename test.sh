#!/bin/bash

clang++ -std=c++14 calculator.cpp -o calculator

function assert-equal() {
    argument=$1
    expect=$2
    actual=$(./calculator $argument)
    if [ ! "$actual" = "$expect" ]; then
        echo "$actual expected, but got $actual"
        exit 1
    fi
}

assert-equal test/ex1.ca "30"
assert-equal test/ex2.ca "1300"
assert-equal test/ex3.ca "245"

echo OK
