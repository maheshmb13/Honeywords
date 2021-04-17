#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>

#define PORT 8090


struct UserDetails{
	int type;
	char username[20];
	char hashedpassword[32];
};

struct UserDetails user;


void computeMD5(char password[]){
	
	FILE *temp = fopen("pwd.txt", "w");
	fprintf(temp, "%s\n", password);
	fclose(temp);

	FILE *fp;
	fp = popen("md5deep pwd.txt", "r");
	fscanf(fp,"%s", user.hashedpassword);
	pclose(fp);

	if(remove("pwd.txt") == 0){}
}

int main(int argc, char const *argv[])
{
	int type;
	
	printf("Press\n");
	printf("1 - Login\n2 - Register\n");
	
	scanf("%d", &type);
	char password[12];
	if(type==1){
		printf("Username: ");
		scanf("%s", user.username);
		printf("Password: ");
		scanf("%s", password);

		computeMD5(password);
		user.type=1;
	}
	else{
		printf("New Username: ");
		scanf("%s", user.username);
		printf("New Password: ");
		scanf("%s", password);

		computeMD5(password);
		user.type=2;
	}


	struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET,SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
        printf("\nAddress not supported \n"); 
        return -1; 
    } 

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    struct timeval tv;
    // 1-second timeout on recv calls with socket
    tv.tv_sec = 1;
    tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

	while(1){
  		send(sock, &user, sizeof user, 0);

  		int ack=0;
  		int valread = read(sock, &ack, sizeof ack);
  		if(valread>0 && ack==1){
  			break;
  		}
  	}
  	
  	int status;
  	int val2 = read(sock, &status, sizeof status);
  	if(status == 1){
  		printf("Successfully Logged in\n");
  	}
  	if(status == 0){
  		printf("User not found");
  	}
  	if(status == 2){
  		printf("Password breach! Matched with a honeyword!\n");
  	}
  	close(sock);
	return 0;
}