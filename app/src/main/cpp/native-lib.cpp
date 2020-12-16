#include <jni.h>
#include <string>
#include <vector>
#include <numeric>
#include <future>
#include <thread>
#include <list>

using namespace std;

class NativeObjectExample {
public:
    int a = 0;
    int b = 1;

    NativeObjectExample() = default;

    NativeObjectExample(int x1, int x2) {
        a = x1;
        b = x2;
    }

    int NativeAddition(int x1, int x2) const {
        auto lambda = [](int p1, int p2) -> int { return p1 + p2; };
        return lambda(x1, x2);
    }
};



// retourne une chaine
extern "C" JNIEXPORT jstring JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_stringFromJNI(
        JNIEnv *env,
        jobject javaThis) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// retourne un int
extern "C" JNIEXPORT jint JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_intFromJNI(
        JNIEnv *env,
        jobject javaThis) {
    return 10;
}

// retourne un jByteArray (jbyte = char en C++)
extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_arrayFromJNI(
        JNIEnv *env,
        jobject javaThis) {

    int version_len = 5;

    // Prior to C++17, no share_ptr of array
    auto charTab = new char[version_len];
    charTab[0] = 's';
    charTab[1] = 'a';
    charTab[2] = 'l';
    charTab[3] = 'u';
    charTab[4] = 't';

    // conversion char * to jbyte
    auto *jByteTab = new jbyte[version_len];
    for (int i = 0; i <= version_len; i++) {
        jByteTab[i] = (jbyte) charTab[i];
    }

    // conversion to jcharArray and return
    jbyteArray jByteArray = env->NewByteArray(version_len);
    env->SetByteArrayRegion(jByteArray, 0, version_len, jByteTab);

    delete[] charTab;
    delete[] jByteTab;

    return jByteArray;
}

// retourne un objet Kotlin
extern "C" JNIEXPORT jobject JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_objectFromJNI(
        JNIEnv *env,
        jobject javaThis) {

    auto jClassRef = env->FindClass("com/amarchaud/nativeclass/ObjectExample");
    auto jConstructorRef = env->GetMethodID(jClassRef, "<init>", "(II)V");
    return env->NewObject(jClassRef, jConstructorRef,
                          2, 3);
}

// appelle une methode d'un objet Kotlin
extern "C" JNIEXPORT jint JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_addFromKotlin(
        JNIEnv *env,
        jobject javaThis, jobject jObjectExample, jint x1, jint x2) {

    auto jClassRef = env->FindClass("com/amarchaud/nativeclass/ObjectExample");
    auto jMethodRef = env->GetMethodID(jClassRef, "Addition", "(II)I");
    if (jMethodRef == nullptr)
        return -999;

    // auto jMethodRefFoo = env->GetMethodID(jClassRef, "SetterFoo", "(I)V");
    // just for fun : get attribut of Kotlin class, even it is private !
    auto jFieldRefA = env->GetFieldID(jClassRef, "a", "I");
    env->SetIntField(jObjectExample, jFieldRefA, x1);
    auto jFieldRefB = env->GetFieldID(jClassRef, "b", "I");
    env->SetIntField(jObjectExample, jFieldRefB, x2);

    return env->CallIntMethod(jObjectExample, jMethodRef,
                              env->GetIntField(jObjectExample, jFieldRefA),
                              env->GetIntField(jObjectExample, jFieldRefB));
    // or
    //return env->CallIntMethod(jObjectExample, jMethodRef, x1, x2);
}

// appelle une methode d'un objet C++ et remonte le resultat
extern "C" JNIEXPORT jint JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_addFromNative(
        JNIEnv *env,
        jobject javaThis, jint x1, jint x2) {

    unique_ptr<NativeObjectExample> nativeObjectExample = make_unique<NativeObjectExample>();
    return nativeObjectExample->NativeAddition(x1, x2);
}

// appelle une methode d'un objet Kotlin
extern "C" JNIEXPORT jobject JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_fusionFromKotlin(
        JNIEnv *env,
        jobject javaThis, jobject jObjectExample, jobject arrayList1, jobject arrayList2) {

    auto jClassRef = env->FindClass("com/amarchaud/nativeclass/ObjectExample");
    auto jFusionMethodRef = env->GetMethodID(jClassRef, "FusionList",
                                             "(Ljava/util/ArrayList;Ljava/util/ArrayList;)Ljava/util/ArrayList;");
    if (jFusionMethodRef == nullptr)
        return nullptr;

    return env->CallObjectMethod(jObjectExample, jFusionMethodRef, arrayList1, arrayList2);
}

/*
 * Test d'un future et d'un thread pour la conversion Kotlin ArrayList to std::List
 */
void jArrayListToNativeListFuture(JavaVM *jm,
                            jobject arrayList,
                            jint listSize,
                            list<string> &res) {

    JNIEnv *env = nullptr;
    jm->AttachCurrentThread(&env, nullptr);

    auto jArrayListRef = env->FindClass("java/util/ArrayList");
    auto java_util_ArrayList_get = env->GetMethodID(jArrayListRef, "get",
                                                    "(I)Ljava/lang/Object;");

    for (int i = 0; i < listSize; i++) {
        auto element = static_cast<jstring>(env->CallObjectMethod(arrayList,
                                                                  java_util_ArrayList_get, i));
        auto pchars = env->GetStringUTFChars(element, nullptr);
        res.emplace_back(pchars);
    }


    jm->DetachCurrentThread();
}


void jArrayListToNativeListThread(JavaVM *jm, int listSize, jobject arrayList, list<string> &res) {
    JNIEnv *env = nullptr;
    jm->AttachCurrentThread(&env, nullptr);

    auto jArrayListRef = env->FindClass("java/util/ArrayList");
    auto java_util_ArrayList_get = env->GetMethodID(jArrayListRef, "get",
                                                    "(I)Ljava/lang/Object;");

    for (int i = 0; i < listSize; i++) {
        auto element = static_cast<jstring>(env->CallObjectMethod(arrayList,
                                                                  java_util_ArrayList_get, i));
        auto pchars = env->GetStringUTFChars(element, nullptr);
        res.emplace_back(pchars);
    }


    jm->DetachCurrentThread();
};

extern "C" JNIEXPORT jobject JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_fusionNative(
        JNIEnv *env,
        jobject javaThis, jobject arrayList1, jobject arrayList2) {

    // solution 1 : appeler des éléments Native
    auto jArrayListRef = env->FindClass("java/util/ArrayList");
    auto jArrayListConstructorRef = env->GetMethodID(jArrayListRef, "<init>", "()V");


    /*
     // solution 1  ; n'utiliser que les objets Java
         auto jArrayListAddAllRef = env->GetMethodID(jArrayListRef, "addAll",
                                                "(Ljava/util/Collection;)Z");
    auto jArrayList3 = env->NewObject(jArrayListRef, jArrayListConstructorRef);
    env->CallBooleanMethod(jArrayList3, jArrayListAddAllRef, arrayList1);
    env->CallBooleanMethod(jArrayList3, jArrayListAddAllRef, arrayList2);
    return jArrayList3;
    */

    // solution 2 : créer une list<string> et la retourner en la convertissant en objet Java
    auto java_util_ArrayList_size = env->GetMethodID(jArrayListRef, "size", "()I");
    auto java_util_ArrayList_get = env->GetMethodID(jArrayListRef, "get", "(I)Ljava/lang/Object;");
    jint len1 = env->CallIntMethod(arrayList1, java_util_ArrayList_size);
    jint len2 = env->CallIntMethod(arrayList2, java_util_ArrayList_size);

    list<string> returnedListNative;

    JavaVM *jm = nullptr;
    env->GetJavaVM(&jm);


    auto globalArrayList1 = env->NewGlobalRef(arrayList1);
    thread convert1(&jArrayListToNativeListThread, jm, len1, globalArrayList1,
                    ref(returnedListNative));

    auto globalArrayList2 = env->NewGlobalRef(arrayList2);
    future<void> convert2 = async(launch::async, jArrayListToNativeListFuture,
                                  jm,
                                  globalArrayList2,
                                  len2,
                                  ref(returnedListNative));


    convert1.join();
    convert2.get();

    env->DeleteGlobalRef(globalArrayList1);
    env->DeleteGlobalRef(globalArrayList2);

    auto jArrayListAddRef = env->GetMethodID(jArrayListRef, "add",
                                             "(Ljava/lang/Object;)Z");

    // converstion std::list en ArrayList !
    auto jArrayList3 = env->NewObject(jArrayListRef, jArrayListConstructorRef);
    for (const auto &oneStr : returnedListNative) {
        env->CallBooleanMethod(jArrayList3, jArrayListAddRef, env->NewStringUTF(oneStr.c_str()));
    }

    return jArrayList3;
}

int accumulateNormal(
        vector<int>::iterator first,
        vector<int>::iterator last) {
    return accumulate(first, last, 0);
}


// test d'un thread
extern "C" JNIEXPORT jint JNICALL
Java_com_amarchaud_ui_nativeexample_MainFragmentViewModel_threadFromJNI(
        JNIEnv *env,
        jobject javaThis) {

    vector<int> numbers = {1, 2, 3, 4, 5, 6};
    future<int> testFuture = async(launch::async, accumulateNormal, numbers.begin(), numbers.end());
    return testFuture.get();
}