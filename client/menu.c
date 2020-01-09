#include "client.h"

void select_menu(int sockfd, msg_t* msg)
{
	int choice;	//选择一级菜单
	while(1){
		printf("*************** 欢迎使用在线小词典 *********************\n");
		printf("*    1.register	  2.login		3.quit *\n");
		printf("*****************************************************\n");
		printf("please choose:");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 1:
				do_register(sockfd, msg);
				break;
			case 2:
				if(!do_login(sockfd, msg)){
					//如果登录成功，跳到二级菜单
					goto next;
				}
				break;
			case 3:
				close(sockfd);
				exit(0);
				break;
			default:
				printf("Invalid data cmd\n");
				break;
		}
	}
//二级菜单
next:
	while(1){
		printf("**************************************************\n");
		printf("** 1.query word \t\t2.history record\t3.quit** \n");
		printf("**************************************************\n");
		printf("please choose: ");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 1:
				do_query(sockfd,msg);
				break;
			case 2:
				do_history(sockfd,msg);
				break;
			case 3:
				close(sockfd);
				exit(0);
				break;
			default:
				printf("Invalid data cmd\n");
				break;
		}

	}
}
