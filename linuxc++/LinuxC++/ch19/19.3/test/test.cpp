#include <openssl/evp.h>
#include <string.h>
#define FAILURE -1
#define SUCCESS 0

int do_encrypt(const EVP_CIPHER *type, const char *ctype)  
{  
	unsigned char outbuf[1024];  
	int outlen, tmplen;  
	unsigned char key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };  
	unsigned char iv[] = { 1, 2, 3, 4, 5, 6, 7, 8 };  
	char intext[] = "Helloworld";  
	EVP_CIPHER_CTX ctx;  
	FILE *out;  
	EVP_CIPHER_CTX_init(&ctx);  
	EVP_EncryptInit_ex(&ctx, type, NULL, key, iv);  
      
	if (!EVP_EncryptUpdate(&ctx, outbuf, &outlen, (unsigned char*)intext, (int)strlen(intext))) {  
		printf("EVP_EncryptUpdate\n");  
		return FAILURE;  
	}     
      
	if (!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {  
		printf("EVP_EncryptFinal_ex\n");  
		return FAILURE;  
	}     
      
	outlen += tmplen;  
	EVP_CIPHER_CTX_cleanup(&ctx);  
      
	out = fopen("./cipher.dat", "wb+");  
	fwrite(outbuf, 1, outlen, out);  
	fflush(out);  
	fclose(out);  
	return SUCCESS;  
}  

int do_decrypt(const EVP_CIPHER *type, const char *ctype)  
{  
	unsigned char inbuf[1024] = { 0 };  
	unsigned char outbuf[1024] = { 0 };  
	int outlen, inlen, tmplen;  
	unsigned char key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };  
	unsigned char iv[] = { 1, 2, 3, 4, 5, 6, 7, 8 };  
  
	EVP_CIPHER_CTX ctx;  
	FILE *in = NULL;  
	EVP_CIPHER_CTX_init(&ctx);  
	EVP_DecryptInit_ex(&ctx, type, NULL, key, iv);  
  
	in = fopen("cipher.dat", "r");  
	inlen = fread(inbuf, 1, sizeof(inbuf), in);  
	fclose(in);  
  
	printf("Readlen: %d\n", inlen);  
	if (!EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) {  
		printf("EVP_DecryptUpdate\n");  
		return FAILURE;  
	}  
  
	if (!EVP_DecryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {  
		printf("EVP_DecryptFinal_ex\n");  
		return FAILURE;  
	}  
  
	outlen += tmplen;  
	EVP_CIPHER_CTX_cleanup(&ctx);  
  
	printf("Result: \n%s\n", outbuf);  
  
	return SUCCESS;  
} 

int main(int argc, char *argv[])  
{  
	do_encrypt(EVP_des_cbc(), "des-cbc");  
	do_decrypt(EVP_des_cbc(), "des-cbc");  
      
	do_encrypt(EVP_des_ede_cbc(), "des-ede-cbc");  
	do_decrypt(EVP_des_ede_cbc(), "des-ede-cbc");  
      
	do_encrypt(EVP_des_ede3_cbc(), "des-ede3-cbc");  
	do_decrypt(EVP_des_ede3_cbc(), "des-ede3-cbc");  
      
	return 0;  
}  
