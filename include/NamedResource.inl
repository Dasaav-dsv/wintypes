namespace WinTypes {
    // Internal resource structure used for resource mapping
    template <typename T>
    struct NamedResourceMap {
        T* resptr;
        HANDLE owner;
        SharedMutex srw;
    };
}