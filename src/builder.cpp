#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

const char D = '\1';

int main() {
    string bf_pre = "X", bf_post = "X";

    string c_pre = "#include <stdio.h>\nchar* s = \"";
    string c_post = 
        "\";\nint main() {\n"
        "    int n=0; char *p=s, *e;\n"
        "    while(n<4){ if(*p++=='\\1') n++; }\n"
        "    for(e=p; *e!='\\1'; e++); fwrite(p,1,e-p,stdout);\n"
        "    printf(\"s = \\\"\");\n"
        "    for(int i=0; s[i]; i++) printf(\"\\\\x%02x\", (unsigned char)s[i]);\n"
        "    printf(\"\\\"\\n\");\n"
        "    p=e+1; for(e=p; *e!='\\1'; e++); fwrite(p,1,e-p,stdout);\n"
        "    return 0;\n"
        "}\n";

    string py_pre = 
        "import sys\n"
        "def main():\n"
        "    p = s.split('\\1')\n"
        "    sys.stdout.write(p[6])\n"
        "    sys.stdout.write(', '.join(map(str, map(ord, s))) + ', 0\\n')\n"
        "    sys.stdout.write(p[7])\n";
    string py_post = "if __name__ == '__main__': main()\n";

    string asm_pre = 
        "section .text\n"
        "global _start\n"
        "_start:\n"
        "    mov rsi, s\n"
        "    mov rdi, 8\n"
        "    call _f\n"
        "    call _p\n"
        "    mov rax, 1\n"
        "    mov rdi, 1\n"
        "    mov rsi, r1\n"
        "    mov rdx, 20\n"
        "    syscall\n"
        "    mov rax, 1\n"
        "    mov rdi, 1\n"
        "    mov rsi, s\n"
        "    mov rdx, s_len\n"
        "    syscall\n"
        "    mov rax, 1\n"
        "    mov rdi, 1\n"
        "    mov rsi, r2\n"
        "    mov rdx, 6\n"
        "    syscall\n"
        "    mov rsi, s\n"
        "    mov rdi, 9\n"
        "    call _f\n"
        "    call _p\n"
        "    mov rax, 60\n"
        "    xor rdi, rdi\n"
        "    syscall\n"
        "_f: test rdi, rdi\n"
        "    jz .done\n"
        ".loop: cmp byte [rsi], 1\n"
        "    jne .next\n"
        "    dec rdi\n"
        "    jz .found\n"
        ".next: inc rsi\n"
        "    jmp .loop\n"
        ".found: inc rsi\n"
        ".done: ret\n"
        "_p: mov rdx, 0\n"
        ".len: cmp byte [rsi+rdx], 0\n"
        "    je .out\n"
        "    cmp byte [rsi+rdx], 1\n"
        "    je .out\n"
        "    inc rdx\n"
        "    jmp .len\n"
        ".out: mov rax, 1\n"
        "    mov rdi, 1\n"
        "    syscall\n"
        "    ret\n"
        "section .data\n"
        "r1: db '    string s = R', 34, 'QR(', 0\n"
        "r2: db ')QR', 34, ';', 10, 0\n"
        "s: db ";
    string asm_post = "\ns_len equ $ - s - 1\n";

    string cpp_pre = 
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <iomanip>\n"
        "using namespace std;\n"
        "int main() {\n";
    string cpp_post = 
        "    auto get = [&](int n) {\n"
        "        size_t p=0;\n"
        "        for(int i=0; i<n; i++) p = s.find('\\1', p) + 1;\n"
        "        return s.substr(p, s.find('\\1', p) - p);\n"
        "    };\n"
        "    auto to_hex = [&](string st) {\n"
        "        stringstream ss; ss << hex << setfill('0');\n"
        "        for(unsigned char c : st) ss << \"\\\\x\" << setw(2) << (int)c;\n"
        "        return ss.str();\n"
        "    };\n"
        "    string c = get(2) + to_hex(s) + get(3);\n"
        "    int cur = 0;\n"
        "    for(unsigned char b : c) {\n"
        "        while(cur < b) { cout << '+'; cur++; }\n"
        "        while(cur > b) { cout << '-'; cur--; }\n"
        "        cout << '.';\n"
        "    }\n"
        "    return 0;\n"
        "}\n";

    stringstream ss;
    ss << bf_pre << D << bf_post << D << c_pre << D << c_post << D 
       << py_pre << D << py_post << D << asm_pre << D << asm_post << D 
       << cpp_pre << D << cpp_post << D;
    string s = ss.str();

    ofstream f("src/relay.cpp");
    f << cpp_pre << "    string s = R\"QR(" << s << ")QR\";\n" << cpp_post;
    
    return 0;
}
