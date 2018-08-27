/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cctype>
//#include <string>
#include "Packet.h"
#include "LinuxDARwIn.h"

#include "Camera.h"
#include "mjpg_streamer.h"
#include "minIni.h"
#include "LinuxCamera.h"

#define INI_FILE_PATH       "config.ini"

void dostuff(int); 
void error(const char *msg)
{
    perror(msg);
    exit(1);
}


void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

using namespace Robot;

int main(int argc, char *argv[])
{
    // check usage
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    printf( " == Camera Tutorial for DARwIn === \n\n");
    
    // Setting up camera streaming
    /*
    change_current_dir();

    minIni* ini = new minIni(INI_FILE_PATH);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->LoadINISettings(ini);

    mjpg_streamer* streamer = new mjpg_streamer(Camera::WIDTH, Camera::HEIGHT);
   
    int i = 0;
    while(true){ 
        LinuxCamera::GetInstance()->CaptureFrame();
        streamer->send_image(LinuxCamera::GetInstance()->fbuffer->m_RGBFrame);
        printf("send_image %d",i);
        i++;
    }
    */

    // Open ports
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    int portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
   
 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    
    listen(sockfd,5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    std::cout << "Initializing socket server on port " << argv[1] << std::endl;
    
    // Fork processes
    while (1) 
    {
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if (newsockfd < 0) error("ERROR on accept");
        
        int pid = fork();
        if (pid < 0) error("ERROR on fork");
        if (pid == 0) {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else 
            close(newsockfd);
    } /* end of while */
    close(sockfd);
    return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
    //////////////////// Framework Initialize ////////////////////////////
    LinuxCM730 linux_cm730("/dev/ttyUSB0");
    CM730 cm730(&linux_cm730);
    if(cm730.Connect() == false)
    {
        printf("Fail to connect CM-730!\n");
        exit(0);
    }
    else 
        std::cout << "Framework initialize success." << std::endl;
    /////////////////////////////////////////////////////////////////////
    
    
    
    // Setting up camera streamingi
    /*
    change_current_dir();

    minIni* ini = new minIni(INI_FILE_PATH);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->LoadINISettings(ini);

    mjpg_streamer* streamer = new mjpg_streamer(Camera::WIDTH, Camera::HEIGHT);
    */

    while(true){
        dxl_comm_packet pack;
        char infostream[256];    
        bzero(infostream,256);
        int n = read(sock,infostream,256);
        if (n < 0) error("ERROR reading from socket");        
        
        char buff[256];
        int buff_index=0;
        int pack_index=0;

        for(int i=0; i<256; i++){
            char mychar = infostream[i];
            if(isalpha(mychar)){
                pack[pack_index] = atoi(buff);
                std::cout << pack[pack_index] << std::endl;
                bzero(buff,256);
                buff_index = 0; 
                pack_index++;
           
                if(mychar == 'x') break;
                
                continue;
            }
            else {
               buff[buff_index] = mychar;
               buff_index++;
            }
        }
        cm730.WriteWord(pack[0], MX28::P_GOAL_POSITION_L,pack[1],0); 
        cm730.WriteWord(pack[2], MX28::P_GOAL_POSITION_L,pack[3],0);

        // Send image
//        LinuxCamera::GetInstance()->CaptureFrame();
//        streamer->send_image(LinuxCamera::GetInstance()->fbuffer->m_RGBFrame);
    }
}
