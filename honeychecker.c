#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <time.h>

#define PORT 8000

int k = 6;

int main(int argc, char const *argv[])
{
	int server_fd, new_socket; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address);


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);  

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    if ((new_socket = accept(server_fd,(struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 	

    int type;
    int val = read(new_socket, &type, sizeof type);

    if(type==1){
	    int matchedIndices[k];
	    char username[20];
	    while(1){
	    	int valread2 = read(new_socket, &username, sizeof username);
		    int valread = read(new_socket, &matchedIndices, sizeof matchedIndices);

		    int ack=0;
		    if(valread>=0 && valread2>=0){
		    	printf("Recieved without error.\n");
		    	ack=1;
		    	send(new_socket, &ack, sizeof ack, 0);
		    	break;
		    }
		    else{
		    	bzero(&matchedIndices, sizeof matchedIndices);
		    	printf("Data corrupted! Found an error.\n");
		    }
		}	
		// printf("%s\n", username);

		FILE *fp = fopen("TrueIndices.txt","r");
		
		char check[20];
		int index;
		
		while (fscanf(fp, "%s", check) != EOF){
	       	if(fscanf(fp, "%d", &index)){}
	       	if(strcmp(username, check)==0){
	       		break;
	       	}
	    }
	    fclose(fp);

	    int present=0;

		for(int i=0; i<k; i++){
			// printf("%d ", matchedIndices[i]);
			if(matchedIndices[i] == index){
				present=1;
			}
		}
		send(new_socket, &present,sizeof present, 0);
		close(new_socket);

	}
	else if(type==2){
		char username[20];
		int correctIndex;
		while(1){
	    	int valread2 = read(new_socket, &username, sizeof username);
		    int valread = read(new_socket, &correctIndex, sizeof correctIndex);

		    int ack=0;
		    if(valread>=0 && valread2>=0){
		    	printf("Recieved without error.\n");
		    	ack=1;
		    	send(new_socket, &ack, sizeof ack, 0);
		    	break;
		    }
		    else{
		    	bzero(&username, sizeof username);
		    	printf("Data corrupted! Found an error.\n");
		    }
		}	
		FILE *fp = fopen("TrueIndices.txt","a+");
		fprintf(fp, "%s %d\n", username, correctIndex);
		fclose(fp);

		close(new_socket);
	}

	return 0;
}