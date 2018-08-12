#if !__has_include(<filesystem>)
    #error "Your compiler doesn't have full C++17 support - lack of <filesystem>."
#endif

int main() {}