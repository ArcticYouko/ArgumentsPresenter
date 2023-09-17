#include <Windows.h>
#include <strsafe.h>

int Main(int argc, wchar_t** argv) {
    size_t size = 1;
    for (size_t i = 0; i < argc; i++) {
        size_t length;
        if (FAILED(StringCchLengthW(argv[i], STRSAFE_MAX_CCH, &length))) {
            return -1;
        }
        size += length + 2;
    }
    wchar_t* buffer = (wchar_t*) LocalAlloc(LMEM_FIXED, size * sizeof(wchar_t));
    if (!buffer) {
        return -1;
    }
    buffer[0] = 0;
    for (size_t i = 0; i < (size_t) argc - 1; i++) {
        StringCchCatW(buffer, size, argv[i]);
        StringCchCatW(buffer, size, L"\r\n");
    }
    StringCchCatW(buffer, size, argv[argc - 1]);
    MessageBoxW(NULL, buffer, L"Arguments Presenter", MB_ICONINFORMATION);
    LocalFree(buffer);
    return 0;
}

int ParseCommandLine(const wchar_t* command, int (*handler)(int, wchar_t**)) {
    if (!command || !*command) {
        return handler(0, NULL);
    }
    int argc;
    wchar_t** argv = CommandLineToArgvW(command, &argc);
    if (!argv) {
        return handler(0, NULL);
    }
    int result = handler(argc, argv);
    LocalFree(argv);
    return result;
}

int EntryPoint() {
    return ParseCommandLine(GetCommandLineW(), Main);
}
