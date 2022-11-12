#!/bin/bash

function run_pos
{
    for i in {1..28}; do
        ./pos_comp.sh "pos_"$i"_in.txt" "pos_"$i"_out.txt"
        if [ "$?" -eq 0 ]; then
            echo "Test pos_$i: PASS"
        else
            echo "Test pos_$i: FAIL"
        fi
    done
}

function run_neg
{
    for i in {1..12}; do
        ./neg_comp.sh "neg_"$i"_in.txt" "neg_"$i"_out.txt"
        if [ "$?" -eq 0 ]; then
            echo "Test neg_$i: PASS"
        else
            echo "Test neg_$i: FAIL"
        fi
    done
}

run_pos
run_neg
