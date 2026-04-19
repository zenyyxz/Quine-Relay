#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
int main() {
    string s = R"QR(XX#include <stdio.h>
char* s = "";
int main() {
    int n=0; char *p=s, *e;
    while(n<4){ if(*p++=='\1') n++; }
    for(e=p; *e!='\1'; e++); fwrite(p,1,e-p,stdout);
    printf("s = \"");
    for(int i=0; s[i]; i++) printf("\\x%02x", (unsigned char)s[i]);
    printf("\"\n");
    p=e+1; for(e=p; *e!='\1'; e++); fwrite(p,1,e-p,stdout);
    return 0;
}
import sys
def main():
    p = s.split('\1')
    sys.stdout.write(p[6])
    sys.stdout.write(', '.join(map(str, map(ord, s))) + ', 0\n')
    sys.stdout.write(p[7])
if __name__ == '__main__': main()
section .text
global _start
_start:
    mov rsi, s
    mov rdi, 8
    call _f
    call _p
    mov rax, 1
    mov rdi, 1
    mov rsi, r1
    mov rdx, 20
    syscall
    mov rax, 1
    mov rdi, 1
    mov rsi, s
    mov rdx, s_len
    syscall
    mov rax, 1
    mov rdi, 1
    mov rsi, r2
    mov rdx, 6
    syscall
    mov rsi, s
    mov rdi, 9
    call _f
    call _p
    mov rax, 60
    xor rdi, rdi
    syscall
_f: test rdi, rdi
    jz .done
.loop: cmp byte [rsi], 1
    jne .next
    dec rdi
    jz .found
.next: inc rsi
    jmp .loop
.found: inc rsi
.done: ret
_p: mov rdx, 0
.len: cmp byte [rsi+rdx], 0
    je .out
    cmp byte [rsi+rdx], 1
    je .out
    inc rdx
    jmp .len
.out: mov rax, 1
    mov rdi, 1
    syscall
    ret
section .data
r1: db '    string s = R', 34, 'QR(', 0
r2: db ')QR', 34, ';', 10, 0
s: db 
s_len equ $ - s - 1
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
int main() {
    auto get = [&](int n) {
        size_t p=0;
        for(int i=0; i<n; i++) p = s.find('\1', p) + 1;
        return s.substr(p, s.find('\1', p) - p);
    };
    auto to_hex = [&](string st) {
        stringstream ss; ss << hex << setfill('0');
        for(unsigned char c : st) ss << "\\x" << setw(2) << (int)c;
        return ss.str();
    };
    string c = get(2) + to_hex(s) + get(3);
    int cur = 0;
    for(unsigned char b : c) {
        while(cur < b) { cout << '+'; cur++; }
        while(cur > b) { cout << '-'; cur--; }
        cout << '.';
    }
    return 0;
}
)QR";
    auto get = [&](int n) {
        size_t p=0;
        for(int i=0; i<n; i++) p = s.find('\1', p) + 1;
        return s.substr(p, s.find('\1', p) - p);
    };
    auto to_hex = [&](string st) {
        stringstream ss; ss << hex << setfill('0');
        for(unsigned char c : st) ss << "\\x" << setw(2) << (int)c;
        return ss.str();
    };
    string c = get(2) + to_hex(s) + get(3);
    int cur = 0;
    for(unsigned char b : c) {
        while(cur < b) { cout << '+'; cur++; }
        while(cur > b) { cout << '-'; cur--; }
        cout << '.';
    }
    return 0;
}
