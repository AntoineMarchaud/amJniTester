package com.amarchaud.ui.nativeexample

import android.content.Context
import com.amarchaud.base.BaseViewModel
import com.amarchaud.nativeclass.ObjectExample

class MainFragmentViewModel(context: Context) : BaseViewModel(context) {

    init {

        val aObjectExample = ObjectExample()

        val stringFromJNI = stringFromJNI()
        println("${MainFragment.TAG} stringFromJNI : $stringFromJNI")

        val intFromJNI: Int = intFromJNI()
        println("${MainFragment.TAG} intFromJNI : $intFromJNI")

        val arrayFromJNI: ByteArray = arrayFromJNI()
        println("${MainFragment.TAG} arrayFromJNI : ${String(arrayFromJNI)}")

        val objectFromJNI: ObjectExample = objectFromJNI()
        println("${MainFragment.TAG} objectFromJNI : ${objectFromJNI.Addition(1, 2)}")

        // appele méthode addition Kotlin
        val addFromKotlin: Int = addFromKotlin(aObjectExample, 100, 200)
        println("${MainFragment.TAG} addFromKotlin : $addFromKotlin")

        // appele méthode addition native
        val addFromNative: Int = addFromNative(100, 200)
        println("${MainFragment.TAG} addFromNative : $addFromNative")

        // test de la méthode fusion
        val l1: ArrayList<String> = arrayListOf("1", "2", "3", "4", "5")
        val l2: ArrayList<String> = arrayListOf("toto", "titi")

        val fusionKotlin: ArrayList<String> = fusionFromKotlin(aObjectExample, l1, l2)
        println("${MainFragment.TAG} fusionKotlin : $fusionKotlin")

        val fusionNative: ArrayList<String> = fusionNative(l1, l2)
        println("${MainFragment.TAG} fusionNative : $fusionNative")

        // thread
        val threadFromJNI: Int = threadFromJNI()
        println("${MainFragment.TAG} threadFromJNI : $threadFromJNI")
    }

    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private external fun stringFromJNI(): String
    private external fun intFromJNI(): Int
    private external fun arrayFromJNI(): ByteArray
    private external fun objectFromJNI(): ObjectExample

    private external fun addFromKotlin(ref: ObjectExample, x1: Int, x2: Int): Int
    private external fun fusionFromKotlin(
        ref: ObjectExample,
        l1: ArrayList<String>,
        l2: ArrayList<String>
    ): ArrayList<String>

    private external fun fusionNative(
        l1: ArrayList<String>,
        l2: ArrayList<String>
    ): ArrayList<String>
    private external fun addFromNative(x1: Int, x2: Int): Int

    private external fun threadFromJNI(): Int
}