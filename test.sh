#!/bin/bash

function assert-equal() {
    argument="$1"
    expect="$2"
    actual=$(./calculator "$argument" 2>&1)
    if [ ! "$actual" = "$expect" ]; then
        echo "$argument, $expect expected, but got $actual"
        exit 1
    fi
}

assert-equal test/add.ca 30
assert-equal test/add_prod.ca 1300
assert-equal test/and.ca 0
assert-equal test/or.ca 1
assert-equal test/eq_neq.ca 2
assert-equal test/le.ca 1
assert-equal test/mod.ca 2
assert-equal test/name_error.ca "Name error, no such a variable a"
assert-equal test/syntax_error.ca "Syntax error, expected =, but got =="
assert-equal test/loop.ca 55
assert-equal test/nested_for.ca 150
assert-equal test/if_true.ca 10
assert-equal test/if_false.ca 5
assert-equal test/if_elseif_else.ca 2
assert-equal test/func.ca 25
# assert-equal test/func_var_arg.ca 15
# assert-equal test/hello.ca "Hello, world!"
# assert-equal test/builtin.ca 5

echo OK
