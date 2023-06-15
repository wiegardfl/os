int main (void) {
int size = 1000000;
int A[size];
int B[size];
int C[size];
for (int i = 0; i < size; i++){
A[i] = i;
B[i] = size - i;
}
// Vektor-Addition
for (int i = 0; i < size; i++) {
C[i] = A[i] + B[i];
}
return 0;
}
