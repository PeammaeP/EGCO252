#include <stdio.h>
#include <string.h>

int ispalindrome(char *word, int size) {
    int i = 0;
    
    printf("The String is : ");
    
    printf("%s \n",word);
    
    printf("The size is %d \n",size);
        
    while(i < size) {
        
        if(word[i] != word[size]) {
            printf("Not Palindrome !\n");
            return 0;
        }
        
        i++;
        size--;
    }
    
    printf("Palindrome !\n");
    return 1;
}

int main() {
    char word_1[] = "abcba";
    char word_2[] = "ABCBA";
    char word_3[] = "Abcba";
    char word_4[] = "abcbA";
    char word_5[] = "hello";
    
    ispalindrome(word_1,sizeof(word_1)/sizeof(word_1[0]) - 2);
    ispalindrome(word_2,sizeof(word_2)/sizeof(word_2[0]) - 2);
    ispalindrome(word_3,sizeof(word_3)/sizeof(word_3[0]) - 2);
    ispalindrome(word_4,sizeof(word_4)/sizeof(word_4[0]) - 2);
    ispalindrome(word_5,sizeof(word_5)/sizeof(word_5[0]) - 2);
    
    printf("The size from strlen is %lu\n", strlen(word_1));
    
    ispalindrome(word_1,strlen(word_1) - 1);
    
    return 0;
}
