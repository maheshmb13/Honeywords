#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <time.h>

#define PORT 8090
#define PORT2 8000

int k = 6;

struct UserDetails{
	int type;
	char username[20];
	char hashedpassword[32];
};

struct UserDetails user;


int main(int argc, char const *argv[])
{
	int status;

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

    while(1){
	    int valread = read(new_socket, &user, sizeof user);
	    int ack=0;
	    if(valread>=0){
	    	printf("Recieved without error.\n");
	    	ack=1;
	    	send(new_socket, &ack, sizeof ack, 0);
	    	break;
	    }
	}	
	// close(new_socket);
	// printf("%s %s", user.username, user.hashedpassword);

	//strcpy(user.username, "wssup");
	//user.type=1;
	//strcpy(user.hashedpassword, "mahesh");
	int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

	if ((sock = socket(AF_INET,SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
			return -1; 
	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT2); 

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

	send(sock, &user.type, sizeof user.type, 0);

	if(user.type==1){
		//login
		FILE *fp = fopen("f1.txt","r");
		int sweetIndices[k];
		char word[20];
		int found=0;
		while (fscanf(fp, "%s", word) != EOF)
        {
            for(int i=0; i<k; i++){
            	if(fscanf(fp, "%d", &sweetIndices[i])){}
            }
        	if(strcmp(user.username, word) == 0){
        		found=1;
        		break;
        	}
        	/*
        	for(int i=0; i<k; i++){
        		printf("%d ", sweetIndices[i]);
        	}
        	printf("\n");
        	*/
        	bzero(&sweetIndices, sizeof sweetIndices);
        }

        fclose(fp);

		if(found == 0){
        	// username not found
        	status=0;
        	send(new_socket, &status, sizeof status, 0);
        	close(new_socket);
        }
        else{
        	//sorting the sweet indices array
        	for(int i=0; i<k; i++){
        		for(int j=i+1; j<k; j++){
        			if(sweetIndices[i] > sweetIndices[j]){
        				int temp = sweetIndices[i];
        				sweetIndices[i] = sweetIndices[j];
        				sweetIndices[j] = temp;
        			}
        		}
        	}
        	int in=0;
        	int check;
        	
        	int matchedIndices[k];
        	memset(matchedIndices, -1, sizeof matchedIndices);
        	int boolean = 0;
        	FILE *fp = fopen("f2.txt","r");
        	while (in<k && fscanf(fp, "%d", &check) != EOF)
	        {
	        	printf("%d\n", check);
	        	int num = sweetIndices[in];
	        	char hash[32];
	        	if(fscanf(fp, "%s", hash)){}
	        	printf("%s\n",hash );
	        	if(num == check){
	        		if(strcmp(user.hashedpassword, hash)==0){
	        			matchedIndices[in] = num;
	        			boolean=1;
	        		}
	        		in++;
	        	}
	        }
	        fclose(fp);
	        if(boolean == 1){
	        	//honeycheck
				while(1){
					send(sock, &user.username, sizeof user.username, 0);
			  		send(sock, &matchedIndices, sizeof matchedIndices, 0);

			  		int ack=0;
			  		int valread = read(sock, &ack, sizeof ack);
			  		if(valread>0 && ack==1){
			  			break;
			  		}
			  	}
			  	int present;

			  	int vals = read(sock, &present, sizeof present);

			  	if(present == 1){
			  		printf("Status: Ok\n");
			  		status=1;
        			send(new_socket, &status, sizeof status, 0);
        			close(new_socket);
			  	}
			  	else{
			  		printf("Status: Warning!! Password Breach\n");
			  		status=2;
        			send(new_socket, &status, sizeof status, 0);
        			close(new_socket);
			  	}
			  	close(sock);
	        }
        }
	}

	else if(user.type==2){
		//register
		int line=0;
		FILE *fp = fopen("f2.txt", "r");

		for (char c = getc(fp); c != EOF; c = getc(fp))
        	if (c == '\n')
            	line = line + 1;

        fclose(fp);
        
        fp = fopen("f2.txt", "a+");

        fprintf(fp, "%d %s\n", line,  user.hashedpassword);
        fclose(fp);

        int cIndex = line;

        int sweetIndices[k];

        srand(time(0));
        int upper = line-1;
        int low = 0;
        for(int i = 0; i<k-1; i++){
        	int num = (rand() % (upper - low + 1)) + low;
        	sweetIndices[i] = num;
        }
        int randomIndex = rand()%6;

        sweetIndices[k-1] = sweetIndices[randomIndex];
        sweetIndices[randomIndex] = cIndex; 

        /*
        for(int i=0; i<k; i++){
        	printf("%d ",sweetIndices[i]);
        }
        */

        fp = fopen("f1.txt", "a+");

        fprintf(fp, "%s", user.username);
        for(int i=0; i<k; i++){
        	fprintf(fp, " %d", sweetIndices[i]);
        }
        fprintf(fp, "\n");

        while(1){
			send(sock, &user.username, sizeof user.username, 0);
			send(sock, &cIndex, sizeof cIndex, 0);

			int ack=0;
			int valread = read(sock, &ack, sizeof ack);
			if(valread>0 && ack==1){
				//printf("matched Array sent!\n");
				close(sock);
				break;
			}
		}
	}

	return 0;
}