package com.amarchaud.injection

import androidx.fragment.app.FragmentActivity
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.amarchaud.ui.nativeexample.MainFragmentViewModel


class ViewModelFactory(private val activity: FragmentActivity): ViewModelProvider.Factory {

    override fun <T : ViewModel?> create(modelClass: Class<T>): T {

        if (modelClass.isAssignableFrom(MainFragmentViewModel::class.java)) {
            return MainFragmentViewModel(activity) as T
        }

        throw IllegalArgumentException("Unknown ViewModel class")
    }
}