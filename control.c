
#include "maodv.h"

mmsg_t snd_buff;

int data_test();
int msg_queues_init();

MADR addr1 = 0;
MADR addr2 = 0;

static int qid_test[top_line];

//第一哥参数为控制节点地址，第二个参数为控制指令，第三个参数为组播地址
int main(int argc, char *argv[])
{

    if (argc < 4)
	{
		printf("parameter1: control_addr\nparameter3: m_addr\n");

		addr1 = atoi(argv[1]);
		//return -1;
	}
	else
	{
		//要控制的节点地址
	    addr1 = atoi(argv[1]);
	    memcpy(snd_buff.data,argv[2],5);
		//要加入的组播地址
	    addr2 = atoi(argv[3]);
	}

    msg_queues_init();

    printf("%d %s %d\n",addr1,snd_buff.data,addr2);

    snd_buff.mtype = MMSG_MAODV_CONTROL;
    snd_buff.node = addr2;

	size_t snd_len = 1000;
    while(msgsnd(qid_test[addr1], &snd_buff,snd_len , IPC_NOWAIT) < 0)
	{
		//等待消息队列空间可用时被信号中断
		if(errno == EINTR)
			continue;
		//由于消息队列的msg_qbytes的限制和msgflg中指定IPC_NOWAIT标志，消息不能被发送,即发送队列满，阻塞
		if(errno == EAGAIN)
		{
			printf("snd queue full , blocked\n...clean this queue...\n");
			printf("notice!!! important data may loss !!\n");
			mmsg_t temp_buff;
			while( msgrcv(qid_test[addr1], &temp_buff, snd_len,0,IPC_NOWAIT) != -1 );

		}
		else
		{
			printf("maodv rreq send error!\n");
			return -1;
		}
	}

	return 0;
}

int msg_queues_init()
{

	int i;
	for(i=0;i<top_line;i++)
	{
		int seq = 100 + i;
		key_t key_test = ftok(PATH_CREATE_KEY, seq);
		if(key_test<0)
		{
			printf("get key_test error!\n");
			return -1;
		}
		qid_test[i] = msgget(key_test, IPC_CREAT|QUEUE_MODE);
		if(qid_test[i] == -1)
		{
			printf("get qid_test %d error!\n",i);
			return -1;
		}
		printf("qid_test of %d : %d\n",i,qid_test[i]);
	}
	return 0;
}

int data_test()
{


	return 0;
}

int control_test()
{


	return 0;
}
