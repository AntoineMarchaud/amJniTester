package com.amarchaud.nativeclass

class ObjectExample() {

    private var a: Int = 0
    var b: Int = 1

    constructor(x1: Int, x2: Int) : this() {
        a = x1
        b = x2
    }

    fun SetterFoo(x1: Int) {
        a = x1
        b = x1
    }

    fun Addition(x1: Int, x2: Int): Int {
        return x1 + x2
    }

    fun FusionList(l1: ArrayList<String>, l2: ArrayList<String>): ArrayList<String> {
        val l3 = ArrayList<String>()
        l3.addAll(l1)
        l3.addAll(l2)
        return l3
    }

}