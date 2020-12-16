package com.amarchaud.ui.nativeexample

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.amarchaud.injection.ViewModelFactory
import com.amarchaud.ui.nativeexample.databinding.FragmentMainBinding

class MainFragment : Fragment() {


    private lateinit var viewModel: MainFragmentViewModel
    private lateinit var binding: FragmentMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_main, container, false)

        // init viewModel
        viewModel =
            ViewModelProviders.of(this, ViewModelFactory(requireActivity())).get(MainFragmentViewModel::class.java)

        // Obtain binding for MVVM
        binding =
            DataBindingUtil.setContentView(requireActivity(), R.layout.fragment_main)
        //binding.aViewModelInXml = viewModel
        binding.lifecycleOwner = this

        return view
    }


    companion object {
        const val TAG: String = "MainActivityTag"
    }
}