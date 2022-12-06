

//忽略C标准库函数引发的警告
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
// #include <conio.h>
// #include <io.h>

#include <termios.h>
#include <stdio.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char _getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

//游戏之中可能出现的商品总数目
#define GOODS_LIST_MAX 16


//商品结构体
typedef struct goods goods;
struct goods{

	char * name;

	//自然情况下最低价格
	int min_price;
	//自然情况下最高价格
	int max_price;
};



//玩家结构体
typedef struct player player;
struct player{

	char name[100];

	//现金
	int money;
	//存款
	int deposit;
	//负债
	int debt;
	//生命
	int hp;
	//名望
	int rp;

	//仓库最大容量
	int capacity;
	//商品数量列表
	int my_list[GOODS_LIST_MAX];
	//商品进价列表
	int my_price[GOODS_LIST_MAX];

	//位置，地面还是在地下
	int location;

	//已经过去的天数
	int ndays;
};


//新闻事件结构体
typedef struct news news;
struct news{

	//新闻信息
	char * info;

	//新闻针对的商品ID
	int id;
	//新闻对于价格的影响力
	double impact;
};


//游戏存档结构体
typedef struct data data;
struct data{

	//存档标志表
	int f[3];

	//存档数据表
	player p[3];
};



//商品列表
const goods goods_list[GOODS_LIST_MAX] = {

	{ "劣质移动电源", 2, 18 },
	{ "盗版游戏光盘", 8, 70 },
	{ "塑胶飞机杯", 125, 407 },
	{ "催情迷药", 205, 880 },
	{ "伪劣化妆品", 365, 990 },
	{ "进口香烟", 537, 1200 },
	{ "二手组装笔记本", 804, 1900 },
	{ "遥控玩具飞机", 940, 2800 },
	{ "假白酒（剧毒）", 1532, 4801 },
	{ "锤子手机", 2011, 6010 },
	{ "XBox One游戏机", 4199, 9901 },
	{ "《上海小宝贝》", 6008, 15010 },
	{ "假冒抗癌剂", 10990, 46224 },
	{ "泰国进口古曼童", 40888, 230000 },
	{ "走私汽车", 88556, 393456 },
	{ "金融街二手凶宅", 869777, 5340000 }
};


//新闻事件列表
news news_list[GOODS_LIST_MAX * 4] = {

	{ "《电脑报》：火车站售卖的移动电源充电一次就报废。", 0, -1.5 },
	{ "北京各大学新生入校，智能手机市场火爆，移动电源供不应求。", 0, 2.5 },
	{ "火车站涌入大批外地小贩，劣质移动电源市场竞争激烈卖不出去。", 0, -2.0 },
	{ "火车站出现大批新疆人卖切糕，无人敢去火车站做生意，劣质移动电源疯涨。", 0, 5.0 },

	{ "“中国硅谷”中关村全是卖盗版游戏的村姑，盗版游戏跳楼价大甩卖。", 1, -4.0 },
	{ "盗版玩家：“玩正版游戏的都是傻逼，汉奸卖国贼！”盗版游戏疯涨。", 1, 6.0 },
	{ "某网友：“盗版玩家买不起正版游戏，也吃不起镇江很贵的鱼，都是穷傻逼。”", 1, -5.0 },
	{ "北京市网民乐于网上下载盗版游戏，盗版游戏光盘卖不出去。", 1, -3.0 },

	{ "年后大批中学生拿压岁钱抢购日本动漫飞机杯。", 2, 2.5 },
	{ "《北京生活报》：北京一浮生者因为使用未经过消毒的飞机杯感染梅毒。", 2, -4.0 },
	{ "大量农民工涌入京城，飞机杯成为民工性生活伴侣。", 2, 4.8 },
	{ "吞卡手电风行一时，网友称其比飞机杯更劲爆。", 2, -2.5 },

	{ "《闹个大新闻报》：北京某地下同性会所大批订购催情迷药。", 3, 4.0 },
	{ "《闹个大新闻报》：近日北京市男童右拐案件数量迅速上升。", 3, 2.0 },
	{ "北京某大学生成功合成强力迷药，网上曝光合成迷药教学视频。", 3, -2.0 },
	{ "喵星人：“猫尿的效果和迷药类似，不信你可以试一试。”", 3, -2.5 },

	{ "晚会上谢不疯：“酷！我用假冒伪劣化妆品！”伪劣化妆品疯涨。", 4, 3.0 },
	{ "《北京真理报》社论：“提倡爱美，落到实处”，化妆品涨4倍。", 4, 4.0 },
	{ "微博曝光：“劣质化妆品内含生石灰，具有强烈腐蚀性。”", 4, -3.6 },
	{ "爱国人士：“假冒伪劣化妆品也是国产，”", 4, 1.8 },

	{ "市场上充斥着来自福建的走私香烟。", 5, 6.0 },
	{ "养生专家：“我一直吸进口香烟活到现在还好好的，进口香烟不会致癌。”", 5, 5.0 },
	{ "某养生专家近日因病去世，死于肺癌。", 5, -2.5 },
	{ "某烟民：“有种进口香烟，感觉不下于飞叶子……”", 5, 3.8 },

	{ "知名企业家：“引进洋垃圾有利于促进城乡居民就业，利大于弊。”", 6, 2.8 },
	{ "《闹个大新闻报》：二手笔记本充电时电池意外爆炸，炸死一人。", 6, -2.8 },
	{ "《北京经济小报》：美国限制洋垃圾出口中国，二手笔记本市场前景难测，价格低迷。", 6, -1.8 },
	{ "电脑城商家：这个笔记本预装正版Win 9.8，价格自然贵，不信你查查正版Win 8.1多少钱？", 6, 6.0 },

	{ "叙利亚武装再次击落中国“无人机”疑似淘宝售卖民品。", 7, 3.0 },
	{ "北京某男子利用遥控飞机偷窥邻居洗澡被捕，民众提议禁售遥控飞机。", 7, 2.0 },
	{ "《北京科技报》：无人机可自制，山西某农民自制无人机飞出大气层，掀起无人机自制热潮。", 7, -3.0 },
	{ "大学生淘宝购买遥控飞机冒充毕业设计，遥控飞机供不应求。", 7, 3.0 },

	{ "河南人狂饮假酒，假酒供不应求。", 8, 3.8 },
	{ "山西假酒生产基地遭查处，生产供应环节遭曝光。", 8, -2.5 },
	{ "河南人忙于偷井盖，无暇喝假酒。", 8, -2.0 },
	{ "酒商：“假酒只要卖得贵就是真酒。”假酒价格狂涨。", 8, 4.2 },

	{ "王自如曝光锤子手机质量问题，引来网络骂战，锤子手机小幅度涨价。", 9, 1.5 },
	{ "锤子手机大幅度降价，罗永浩自打脸。", 9, -4.8 },
	{ "罗永浩：“我的手机专为支那人设计。”大批民众疯抢锤子手机。", 9, 3.2 },
	{ "罗永浩：“锤子如果低于2500，我是你孙子，这不是敢不敢的问题。”锤子手机价格疯涨。", 9, 6.2 },

	{ "索尼PS4游戏机销量破千万，XBox One形式不容乐观。", 10, -1.5 },
	{ "广电总局：“大力普及XBox One机顶盒，实现机顶盒现代化。”，XBox One游戏机疯涨。", 10, 3.4 },
	{ "微软：“我的软狗们在哪里？给我买买买！”", 10, 4.5 },
	{ "索狗：“索尼是世界上最高贵的企业，PS4是世界上最高贵的游戏主机，XBox并不高贵。”XBox One价格下跌。", 10, -2.0 },

	{ "医院的秘密报告：“《上海小宝贝》功效甚过伟哥！”禁书涨4倍。", 11, 4.0 },
	{ "美女作者用身体写作，《上海小宝贝》令人欲火难耐。", 11, 3.0 },
	{ "网上书店也不敢卖《上海小宝贝》，禁书可卖天价。", 11, 4.8 },
	{ "《上海小宝贝》性感美女作者削发为尼，禁书价格下跌。", 11, -3.2 },

	{ "《闹个大新闻报》：欧美科研所成功研发新型抗癌剂，保证百分百治愈癌症晚期。", 12, 7.0 },
	{ "日本医界良心近藤诚：“癌症分为真性癌和假性癌，等死是最好的治疗方法。”", 12, -6.0 },
	{ "李一道长：“癌是我们身体的一部分，我们应当庆祝自己患癌。”抗癌剂大跌。", 12, -5.0 },
	{ "某癌症患者：“癌症患者去医院开刀等于自杀，我推荐自行服用抗癌剂抗癌。”", 12, 7.0 },

	{ "某淘宝店主：“我是得道高僧，养古曼童可以帮你心想事成。”，不信佛的人也开始养小鬼。", 13, 4.0 },
	{ "《闹个大新闻报》：“高中女生养小鬼导致精神分裂跳河自杀。”古曼童价格略降。", 13, -2.0 },
	{ "养小鬼者：“古曼童是佛祖，谁敢诋毁我佛，我佛就让他家破人亡。”信佛的人开始养小鬼。", 13, 5.0 },
	{ "匿名网友：“堕过胎的女人养古曼童可以清洗自己的罪过。”，大批女大学生开始养小鬼。", 13, 4.0 },

	{ "《北京经济小报》社论：走私汽车大力推荐汽车消费，汽车价格涨3倍。", 14, 3.0 },
	{ "买不起车的网民：“操你们妈，买国外汽车的都是狗汉奸！”，进口汽车价格低迷。", 14, -2.0 },
	{ "各地爆发反日游行，游行民众怒砸国外轿车，走私汽车无人敢买。", 14, -5.0 },
	{ "中国留学生：“支那国的关税太高，国内走私车很便宜，适合支那豚。”走私汽车价格涨数倍。", 14, 3.0 },

	{ "任总：“房价高？那是浮云。”房价猛涨。", 15, 6.0 },
	{ "著名经济学家：“只有房价高，中国人才能买得起房。”北京房价疯涨5倍。", 15, 8.0 },
	{ "《北京经济小报》：网上曝光北京市凶宅数据库，凶宅价格下跌。", 15, -3.0 },
	{ "号外号外：李总拍桌子了：“这房价谁能买得起？高价的要收归国有无偿发给浮生者！”全国房价瞬间急跌。", 15, -3.2 }
};



//函数前向声明
void game_start( player iplayer );




//32位的随机数产生函数，标准库的rand()函数并不能满足大随机数生成需要
int rand32(){

	int rv = 0;

	rv |= rand() % 256;
	rv <<= 8;

	rv |= rand() % 256;
	rv <<= 8; 

	rv |= rand() % 256;
	rv <<= 8; 

	rv |= rand() % 256;

	//范围为0 ~ 0x7fffffff，不允许出现负数
	rv &= 0x7fffffff;

	return rv;
}


//获得仓库货物类型数量
int get_goods_number( player * pplayer ){

	int num = 0;
	int i;

	for( i = 0; i < GOODS_LIST_MAX; i++ ){

		if( pplayer->my_list[i] != 0 ){

			num++;
		}
	}

	return num;
}


//获得仓库货物数量
int get_goods_stock( player * pplayer ){

	int num = 0;
	int i;

	for( i = 0; i < GOODS_LIST_MAX; i++ ){
	
		num += pplayer->my_list[i];
	}

	return num;
}






//显示游戏作者
void game_author(){

	system( "clear" );

	puts( "游戏作者：宇宙新闻台" );
	puts( "联系方式：QQ.1982775886" );

	_getch();
}



//新游戏，返回一个用户新创建的玩家
player new_game(){

	player new_player = {
		
		"二狗子",

		2000,
		0,
		5500,
		100,
		100,

		100,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

		0,

		1
	};

	system( "clear" );

	printf( "请输入你的姓名：" );

	gets( new_player.name );

	system( "clear" );

	puts( "俺是一个农民。\n" );

	_getch();

	puts( "一个月前因为俺娘得病要去县城开刀，没办法借了村长5500块钱高利贷。\n" );

	_getch();

	puts( "昨天村长带着村里几个恶霸上俺家讨债把俺爹气晕了过去。\n" );

	_getch();

	puts( "为了爹妈，俺要出去挣钱还债。" );

	_getch();

	system( "clear" );

	puts("翌日清晨---村口\n");

	_getch();

	puts( "老爹站在村口挥泪道别：“儿啊，挣不到钱不要紧，家们永远给你开着……”\n" );

	_getch();

	puts( "俺跪下给俺爹磕了个头，心里挂念着还在病床上的俺娘，踏上了去京城的路……" );

	_getch();

	return new_player;
}


//存档数据子项目管理
int save_mngmnt( player * pplayer, data * pdat, int id ){

	int exit_flag = 0;
	int rv = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		printf( "-存档-\n\n", id );

		puts( "保存---1" );
		puts( "读取---2" );
		puts( "删除---3" );
		puts( "退出---0\n" );

		int key = _getch();

		switch( key ){

			case '1':{
				
				if( NULL == pplayer ){

					puts( "你并没有进入游戏，无法存档。" );

					_getch();
				}
				else{

					pdat->f[id] = 0xffffffff;
					pdat->p[id] = * pplayer;

					puts( "存档保存成功。" );

					_getch();
				}

				exit_flag = 1;
					 }break;

			case '2':{

				if( pdat->f[id] == 0 ){

					puts( "此存档为空，无法读取。" );

					_getch();
				}
				else{

					game_start( pdat->p[id] );	
					
					//程序运行到这里表示玩家刚刚退出游戏，返回值为1，上级函数接受到此返回值可以直接退出到主菜单
					rv = 1;
				}

				exit_flag = 1;
					 }break;

			case '3':{

				if( pdat->f[id] == 0 ){

					puts( "存档为空，无法删除。" );

					_getch();
				}
				else{
					pdat->f[id] = 0;

					puts( "存档删除成功。" );

					_getch();
				}

				exit_flag = 1;
					 }break;

			case '0':{

				exit_flag = 1;
					 }break;
		}
	}

	return rv;
}


//存档文件管理
void data_mngmnt( player * pplayer ){

	//定义一个空存档
	data dat = { { 0, 0, 0 } };

	//检查是否存在存档文件
	if( access( "./save.sav", 0 ) != 0 ){

		FILE * fp = NULL;

		//创建一个新的存档文件
		fp = fopen( "./save.sav", "wb" );

		puts( "清空" );

		_getch();

		//写入空存档数据
		fwrite( &dat, sizeof( dat ), 1, fp );

		fclose( fp );
	}


	FILE * fp = NULL;

	fp = fopen( "./save.sav", "rb" );

	//读取存档之中的数据
	fread( &dat, sizeof( dat ), 1, fp );

	fclose( fp );



	//写入方式打开存档
	fp = fopen( "./save.sav", "wb" );

	fseek( fp, 0, SEEK_SET );

	fwrite( &dat, sizeof( dat ), 1, fp );

	//退出标志
	int exit_flag = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		puts( "-存档管理-\n" );

		int i;

		for( i = 0; i < 3; i++ ){

			if( dat.f[i] != 0 ){

				printf( "%s\t---%d\n", dat.p[i].name, i + 1 );
			}
			else{
			
				printf( "空存档\t---%d\n", i + 1 );
			}
		}

		puts( "退出\t---0" );

		int key = _getch();

		if( key >= '1' && key <= '3' ){

			//如果返回值为1，表示玩家进入过游戏，exit_flag为1，直接退出到主菜单
			if( save_mngmnt( pplayer, &dat, key - '1' ) == 1 ){

				exit_flag = 1;
			}

			fseek( fp, 0, SEEK_SET );

			fwrite( &dat, sizeof( dat ), 1, fp );
		}
		else if( key == '0' ){

			exit_flag = 1;
		}
	}

	fclose( fp );
}


//存钱
void save_money( player * pplayer ){

	int n;

	printf( "女营业员：存多少钱？递过来。\n：" );

	scanf( "%d", &n );

	if( 0 == n ){

		puts( "女营业员白了你一眼：请别浪费我的时间，下一个。" );
	}
	else if( n < 0 ){

		puts( "女营业员：贷款去别的窗口，下一个。" );
	}
	else if( n > pplayer->money ){

		puts( "女营业员不屑地瞟了你一眼。" );
	}
	else{

		pplayer->money -= n;
		pplayer->deposit += n;
					
		if( n > 500000 ){
						
			puts( "女营业员：存款业务办理成功，感谢您的光临，请您慢走。" );
		}
		if( n > 1000000 ){

			puts( "女营业员：业务办理好了先生，这是我的名片。" );
		}
		else{

			puts( "女营业员：好了，下一个。" );
		}
	}

	_getch();
}

//取钱
void take_money( player * pplayer ){

	int n;

	printf( "女营业员：取多少钱？\n：" );

	scanf( "%d", &n );

	if( 0 == n ){

		puts( "女营业员白了你一眼：请别浪费我的时间，下一个。" );
	}
	else if( n < 0 ){

		puts( "女营业员：存款去别的窗口。" );
	}
	else if( n > pplayer->deposit ){

		puts( "女营业员带着不屑的表情说道：你并没有那么多钱。" );
	}
	else{

		pplayer->deposit -= n;
		pplayer->money += n;
					
		if( n > 500000 ){
						
			puts( "女营业员：取款业务办理成功，感谢您的光临，请您慢走。" );
		}
		if( n > 1000000 ){

			puts( "女营业员：业务办理好了先生，这是我的名片。" );
		}
		else{

			puts( "女营业员：好了，下一个。" );
		}
	}

	_getch();
}


//银行
void bank_work( player * pplayer ){

	int exit_flag = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		puts( "-低息危险银行欢迎你-\n" );

		puts( "存钱---1" );
		puts( "取钱---2" );
		puts( "贷款---3");
		puts( "离开---4\n" );

		int key = _getch();

		switch( key ){

			case '1':{

				save_money( pplayer );
					 }break;

			case '2':{

				take_money( pplayer );
					 }break;

			case '3':{

					 }break;

			case '4':{

				exit_flag = 1;
					 }break;
		}
	}
}






//出售货物
void sell_goods( player * pplayer, int id, int price ){

	if( pplayer->my_list[id] > 0 ){

		int exit_flag = 0;

		while( 0 == exit_flag ){

			system( "clear" );

			printf( "-你的仓库里一共有 %d 件 %s ，是否全部卖出？-\n\n", pplayer->my_list[id], goods_list[id].name );

			puts( "是  ---1" );
			puts( "否  ---2" );
			puts( "取消---0\n" );


			int key = _getch();

			switch( key ){

				case '1':{

					int income =  pplayer->my_list[id] * price;
					pplayer->my_list[id] = 0;

					pplayer->money += income;

					puts( "货物已经全部售出。" );

					exit_flag = 1;

					_getch();
						 }break;

				case '2':{

					int num;

					printf( "请输入出售数目。\n：" );

					scanf( "%d", &num );

					if( 0 == num ){
						
						puts( "商人：此处不留爷，自有留爷处。" );
					}
					else if( num < 0 ){
	
						puts( "商人：你搞错没有？" );
					}
					else if( num > pplayer->my_list[id] ){

						printf( "%s：俺没有那么多东西……\n", pplayer->name );
					}
					else{
						
						int income = num * price;
						pplayer->my_list[id] -= num;

						pplayer->money += income;

						puts( "货物已经售出。" );
					}

					exit_flag = 1;

					_getch();
						 }break;

				case '0':{

					exit_flag = 1;
						 }break;
			}
		}
	}
	else{
					
		printf( "%s：俺的仓库里没有这种货。", pplayer->name );

		_getch();
	}
}


//购买货物
void buy_goods( player * pplayer, int id, int price ){

	if( pplayer->money + pplayer->deposit < price ){

		printf( "%s：这东西太贵了，俺一件都买不起啊……", pplayer->name );

		_getch();
	}
	else if( pplayer->money < price ){

		printf( "%s：口袋里的现金暂时不够，要不去银行取点钱吧。", pplayer->name );

		_getch();
	}
	else if ( get_goods_stock( pplayer ) >= pplayer->capacity ){

		printf( "%s：仓库满了，要不要租间更大的呢？", pplayer->name );

		_getch();
	}
	else{

		int exit_flag = 0;

		while( 0 == exit_flag ){

			system( "clear" );

			int mnum = pplayer->money / price;
			int cnum = pplayer->capacity - get_goods_stock( pplayer );
			int rnum = ( mnum < cnum )? mnum : cnum;

			printf( "-你目前最多可以买 %d 件 %s ，是否这样做？-\n\n", rnum, goods_list[id].name );

			puts( "是  ---1" );
			puts( "否  ---2" );
			puts( "取消---0\n" );

			int key = _getch();

			switch( key ){

				case '1':{

					int consume = rnum * price;
					
					pplayer->my_price[id] = ( pplayer->my_list[id] * pplayer->my_price[id] + consume ) / ( pplayer->my_list[id] + rnum );

					pplayer->my_list[id] += rnum;

					pplayer->money -= consume;

					

					puts( "货物已经采购。" );

					exit_flag = 1;

					_getch();
						 }break;

				case '2':{

					int num;

					printf( "请输入采购数目。\n：" );

					scanf( "%d", &num );

					if( 0 == num ){
						
						puts( "商家：不买你来谈什么？" );
					}
					else if( num < 0 ){
	
						puts( "商家：你有货源？这个可以详细说说……" );
					}
					else if( num > cnum ){

						printf( "%s：仓库容纳不了这么多货，要不要租间更大的呢？\n", pplayer->name );
					}
					else if( num > mnum ){

						printf( "%s：俺身上的钱不够买这么多……\n", pplayer->name );
					}
					else{
						
						int consume = num * price;

						pplayer->my_price[id] = ( pplayer->my_list[id] * pplayer->my_price[id] + consume ) / ( pplayer->my_list[id] + num );
						
						pplayer->my_list[id] += num;

						pplayer->money -= consume;

						puts( "货物已经采购。" );
					}

					exit_flag = 1;

					_getch();
						 }break;

				case '0':{

					exit_flag = 1;
						 }break;
			}
		}
	}
}


//货物交易
void trade_goods( player * pplayer, int id, int price ){

	int exit_flag = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		printf( "-%s（单价：%d）-\n\n", goods_list[id].name, price );

		puts( "买入---1" );
		puts( "卖出---2" );
		puts( "取消---0\n" );

		int key = _getch();

		switch( key ){

			case '1':{

				buy_goods( pplayer, id, price );
					 }break;

			case '2':{

				sell_goods( pplayer, id, price );
					 }break;

			case '0':{

				exit_flag = 1;
					 }break;
		}
	}
}


//随机新闻驱动
void random_news( int rlist[], int plist[] ){

	int rd = rand32() % GOODS_LIST_MAX;

	system( "clear" );

	puts( "-北京新闻播报-\n" );

	//随机选择新闻
	int news_id = rlist[rd] * 4 + rand32() % 4;

	if( news_list[news_id].impact > 0 ){

		plist[rd] = ( int )( plist[rd] * news_list[news_id].impact );
	}
	else if( news_list[news_id].impact < 0 ){

		plist[rd] = ( int )( plist[rd] / ( -news_list[news_id].impact ) );
	}

	puts( news_list[news_id].info );

	_getch();
}


//随机事件驱动
void random_event( player * pplayer ){

	system( "clear" );

	int id = rand32() % 25;

	switch( id ){

		case 0:{

			if( pplayer->hp < 60 ){

				puts( "由于不注意身体，我被发现晕倒在马路边，好心人拨120电话，将我送到医院。医生要我住院1天。" );
				
				_getch();

				int hf = 2000 + rand32() % 7000;

				if( pplayer->money > hf + 1000 ){

					printf( "120出车费，急救费，住院费，医药费，总计花费 %d 元。\n", hf );

					pplayer->money -= hf;

					_getch();
				}
				else{

					printf( "村长托人帮我垫付了：120出车费，急救费，住院费，医药费，总计花费 %d 元。", hf );
					
					pplayer->debt += hf;

					_getch();
				}
			}
			   }break;

		case 1:{

			if( pplayer->capacity - get_goods_number( pplayer ) >= 5 ){

				puts( "俺老乡回家前把5瓶假白酒（剧毒）给俺了，嘿嘿，天上真的会掉馅饼啊！" );

				pplayer->my_price[8] = ( pplayer->my_list[8] * pplayer->my_price[8] ) / ( pplayer->my_list[8] + 5 );

				pplayer->my_list[8] += 5;

				_getch();
			}
			   }break;

		case 2:{

			puts( "闯红灯，被BTV电视台拍到，死缠着采访并上了头条新闻，名誉受损1点。" );

			pplayer->rp--;

			_getch();
			   }break;

		case 3:{

			puts( "申奥又添新风景，北京来了沙尘暴。健康下降1点。" );

			pplayer->hp--;

			_getch();
			   }break;

		case 4:{

			puts( "北京拥挤的交通让俺心焦，健康下降1点。" );

			pplayer->hp--;

			_getch();
			   }break;

		case 5:{

			puts( "俺怜悯扮演成乞丐的老太太们，现金减少10%。" );

			pplayer->money = ( int )( pplayer->money * 0.9 );

			_getch();
			   }break;

		case 6:{

			puts( "俺在过街地道被人打了蒙棍！健康下降20点。" );

			pplayer->hp -= 20;

			_getch();
			   }break;

		case 7:{

			if( rand32() % ( pplayer->deposit / 100 + 1 ) == 0 ){

				puts( "ATM机取款，被人用假卡调包。存款损失99%。" );

				pplayer->money = ( int )(pplayer->money * 0.01 );

				_getch();
			}
			   }break;

		case 8:{

			puts( "胡同口的一个小青年砸俺一块砖头，健康下降5点。" );

			pplayer->hp -= 5;

			_getch();
			   }break;

		case 9:{

			if( rand32() % 10 <= ( pplayer->money + pplayer->deposit ) / 10000 ){

				int num = rand32() % 4 + 1;

				printf( "厦门的老同学资助俺几部走私汽车，我得到 %d 部汽车，嘿嘿，天上真的会掉馅饼啊。", num );

				pplayer->my_price[14] = ( pplayer->my_list[14] * pplayer->my_price[14] ) / ( pplayer->my_list[14] + num );

				pplayer->my_list[14] += num;
			
				_getch();
			}
			   }break;

		case 10:{

			if( pplayer->money + pplayer->deposit < 300000 ){

				puts( "守自行车的王大婶嘲笑俺没北京户口。健康下降1点。" );

				pplayer->hp--;

				_getch();
			}
				}break;

		case 11:{

			puts( "副主任严肃地说：“晚上别来我家给我送钱。”存款减少5%。" );

			pplayer->deposit = ( int )( pplayer->deposit * 0.9 );

			_getch();
				}break;

		case 12:{

			if( pplayer->hp < 80 ){

				puts( "大街上的两个流氓打了俺！健康下降3点。" );

				pplayer->hp -= 3;

				_getch();
			}
				}break;

		case 13:{

			if( pplayer->money < 10000 ){

				puts( "一群民工打了俺，健康下降10点。" );

				pplayer->hp -= 10;

				_getch();
			}
				}break;

		case 14:{

			puts( "北京臭黑的小河熏着我了，健康下降1点。" );

			pplayer->hp--;

			_getch();
				}break;

		case 15:{

			if( pplayer->rp <98 ){

				puts( "一个大个子碰了俺一下，说：“别挤了！”。现金减少40%。" );

				pplayer->money = ( int )( pplayer->money * 0.6 );

				_getch();
			}
				}break;

		case 16:{

			puts( "两个假保安用电棍电击俺，健康下降3点。" );

			pplayer->hp -= 3;

			_getch();
				}break;

		case 17:{

			if( pplayer->money + pplayer->deposit < 500000 ){

				puts( "三个带红袖章的老太太揪住俺：“你是外地人？罚款！”，现金减少20%。" );

				pplayer->money = ( int )( pplayer->money * 0.8 );

				_getch();
			}
				}break;

		case 18:{

			if( pplayer->money + pplayer->deposit < 500000 ){

				puts( "开小巴的打俺一耳光！健康下降1点。" );

				pplayer->hp--;

				_getch();
			}
				}break;

		case 19:{

			puts( "手机没电，俺打了个公用电话，两个猛男揪住俺：“交市花费，长话附加费，上网费。”。存款减少5%。" );
			
			pplayer->deposit = ( int )( pplayer->deposit * 0.85 );

			_getch();
				}break;

		case 20:{

			puts( "一个汉字在街头拦阻俺：“哥们，给点钱用！”现金减少10%。" );

			pplayer->money = ( int )( pplayer->money * 0.9 );

			_getch();
				}break;

		case 21:{

			puts( "北京高温40度，健康下降1点。" );

			pplayer->hp--;

			_getch();
				}break;

		case 22:{

			puts( "工商局的追俺超过三个胡同，健康下降1点。" );

			pplayer->hp--;

			_getch();
				}break;

		case 23:{

			puts( "北京空气污染得厉害，俺去氧吧吸氧，现金减少5%。" );

			pplayer->money = ( int )( pplayer->money * 0.95 );

			_getch();
				}break;

		case 24:{

			puts( "黑客入侵银行，搞乱了存款账户信息，存款增加20%。" );
			
			pplayer->deposit = ( int )( pplayer->deposit * 1.2 );
			
			_getch();
				}break;
	}
}


void hehe( player * pplayer ){

	system( "clear" );

	puts( "-街角-\n" );

	puts( "街角站着一位神秘老人。" );

	_getch();

	printf( "和他说些什么吧。\n：" );

	char str[100];

	gets( str );

	if( strcmp( str, "我草你妈" ) == 0 ){

		puts( "神秘老人被气死了。从屁眼里流出了一大堆钱。" );

		_getch();

		pplayer->money += 10000000 + rand32() % 10000000;
	}
	else{

		puts( "神秘老人：我草你妈" );

		_getch();

		printf( "%s：唉，你这人怎么骂俺？\n", pplayer->name );

		_getch();

		puts( "神秘老人：呵呵" );

		_getch();
	}
}


//黑市
void black_market( player * pplayer ){

	int exit_flag = 0;
	int rlist[GOODS_LIST_MAX] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int plist[GOODS_LIST_MAX];
	int i;

	//随机化商品列表
	for( i = 0; i < GOODS_LIST_MAX; i++ ){

		int tmp;
		int m = rand32() % GOODS_LIST_MAX;
	
		tmp = rlist[i];
		rlist[i] = rlist[m];
		rlist[m] = tmp;
	}

	//随机化商品价格
	for( i = 0; i < GOODS_LIST_MAX; i++ ){

		plist[i] = goods_list[rlist[i]].min_price + rand32() % ( abs( goods_list[rlist[i]].max_price - goods_list[rlist[i]].min_price ) + 1 );
	}


	//随机触发新闻事件
	if( rand32() % 3 > 0 ){

		random_news( rlist, plist );
	}
	
	//随机触发事件
	if( rand32() % 3 > 0 ){

		random_event( pplayer );
	}

	int hmax = 7 + rand32() % 3;


	while( 0 == exit_flag ){

		system( "clear" );

		printf( "-当地黑市-\t\t\t\t-仓库（%d/%d）现金：%d-\n\n", get_goods_stock( pplayer ), pplayer->capacity, pplayer->money );

		puts( "商品名称\t\t商品价格\t货物名称\t数量\t进价\n" );


		int i = 0, j = 0, k = 0;

		int mmax = get_goods_number( pplayer );

		while( 1 ){

			if( i < hmax ){

				printf( "%s\t---%d\t%d\t\t", goods_list[rlist[i]].name, i + 1, plist[i] );

				i++;
			}
			else{

				printf( "\t\t\t\t\t" );
			}

			if( j < mmax ){

				while( 0 == pplayer->my_list[k] ){

					k++;
				}

				printf( "%s\t%d\t%d\n", goods_list[k].name, pplayer->my_list[k], pplayer->my_price[k] );

				k++;

				j++;
			}
			else{

				printf( "\n" );
			}


			if( i >= hmax && j >= mmax ){

				break;
			}
		}

		printf( "退出\t\t---0\n" );
		
		int key = _getch();

		if( key >= '0' && key <= '0' + hmax ){

			if( '0' == key ){

				exit_flag = 1;
			}
			else{

				trade_goods( pplayer, rlist[key - '0' - 1], plist[key - '0' - 1] );
			}
		}
	}
}



void internet_bar( player * pplayer ){

	if( pplayer->money < 5 ){

		system( "clear" );

		puts( "网吧老板：没钱上什么网，赶快滚。" );

		_getch();
	}
	else{

		pplayer->money -= 5;

		int exit_flag = 0;

		while( 0 == exit_flag ){
		
			system( "clear" );

			puts( "-网吧-\n" );

			puts( "存档\t\t---1" );
			puts( "网赚\t\t---2" );
			puts( "随便看看\t---3" );
			puts( "离开\t\t---0\n" );

			
			int key = _getch();

			switch( key ){

				case '1':{

					data_mngmnt( pplayer );
						 }break;

				case '2':{

					system( "clear" );

					puts( "你开始在淘宝网刷单。（接下来请反复按任意键直到刷单完成）\n" );

					int count = 20 + rand32() % 40;

					int i;

					for( i = 0; i < count; i++ ){

						puts( "……………………………………" );
						_getch();
					}

					system( "clear" );

					int income = 5 + rand() % 20;

					pplayer->money += income;

					printf( "你通过刷单赚到了 %d 元\n", income );

					_getch();
						 }break;

				case '3':{

					puts( "你随意打开了一个小游戏网站。" );

					system( "start http://www.sun0768.com.cn/Game/" );

					_getch();
						 }break;

				case '0':{

					exit_flag = 1;
						 }break;
			}
		}

		puts( "在网吧消费了5元上机费。" );

		_getch();
	}
}



//退出询问
int exit_ask(){

	int exit_flag = 0;
	int rv = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		puts( "-你要退出游戏吗？-\n" );

		puts( "是---1" );
		puts( "否---2" );

		int key = _getch();

		switch( key ){

			case '1':{

				exit_flag = 1;
				rv = 1;
					 }break;

			case '2':{

				exit_flag = 1;
				rv = 0;
					 }break;
		}
	}

	return rv;
}



//讨债
void collecting_debts( player * pplayer ){

	system( "clear" );

	puts( "半天过后---村口\n" );

	//有债务
	if( pplayer->debt > 0 ){

		puts( "村长：小王八蛋，快还钱！" );

		_getch();

		printf( "村长：你还欠我 %d。\n还多少钱：", pplayer->debt );

		int num;

		scanf( "%d", &num );

		//过滤负数非法输入
		if( num < 0 ){

			num = 0;
		}

		//冤大头，输入过多
		if( num > pplayer->debt ){

			puts( "村长：你傻了吧！" );

			_getch();

			//有能力承担
			if( num <= pplayer->money ){

				puts( "村长：啊哈哈，那我们就两清了。" );

				pplayer->money -= num;
				pplayer->debt = 0;

				_getch();
			}
			//无能力承担
			else{

				puts( "村长：不自量力的家伙，给我打。" );

				_getch();

				puts( "几个大汉把你按到，抢光了你身上所有现金。" );

				_getch();

				//足够还债
				if( pplayer->money >= pplayer->debt ){

					puts( "村长：啊哈哈，那我们就两清了。" );

					pplayer->debt = 0;
					pplayer->money = 0;

					_getch();
				}
				else{

					pplayer->debt -= pplayer->money;
					pplayer->money = 0;

					printf( "村长：小王八蛋，你还欠我 %d！", pplayer->debt );

					_getch();
				}
			}
		}
		else{

			//有能力负担
			if( num <= pplayer->money ){

				pplayer->money -= num;
				pplayer->debt -= num;

				if( pplayer->debt == 0 ){

					puts( "村长：啊哈哈，那我们就两清了。" );

					_getch();
				}
				else{

					printf( "村长：小王八蛋，你还欠我 %d！", pplayer->debt );

					_getch();					
				}
			}
			else{

				puts( "村长：不自量力的家伙，给我打。" );

				_getch();

				puts( "几个大汉把你按到，抢光了你身上所有现金。" );

				_getch();

				pplayer->debt -= pplayer->money;
				pplayer->money = 0;

				printf( "村长：小王八蛋，你还欠我 %d！", pplayer->debt );

				_getch();
			}
		}
	}
	

	if( pplayer->money + pplayer->deposit <= 10000 ){

		puts( "村长：瞧你那熊样。" );

		_getch();
	}
	else if( pplayer->money + pplayer->deposit <= 100000 ){

		puts( "村长：小伙子在北京混的怎么样啊？" );

		_getch();
	}
	else if( pplayer->money + pplayer->deposit <= 1000000 ){

		puts( "村长：乡亲们都快来啊，老板回村了！" );

		_getch();
	}
	else if( pplayer->money + pplayer->deposit <= 10000000 ){

		puts( "村长：大哥过年好，你是我的爹，我是你的儿……" );

		_getch();
	}
	else if( pplayer->money + pplayer->deposit > 10000000 ){

		puts( "村长：你觉得俺家翠花怎么样？" );

		_getch();
	}
}


//回家
void go_home( player * pplayer ){

	system( "clear" );

	puts( "-家里-\n" );

	if( pplayer->money < 100000 ){

		puts( "老爹：你娘还在医院里躺着，唉……" );

		_getch();
	}
	else{

		puts( "老爹：你娘已经出院了。" );

		_getch();

		puts( "老娘：儿啊，在北京一个人别累坏了。是时候找个媳妇了。" );

		_getch();
	}
}


//北京站
void beijing_station( player * pplayer ){

	if( pplayer->money < 100 ){

		system( "clear" );

		puts( "检票员：没有买票不准进站！" );

		_getch();

		printf( "%s：俺身上没有带够买票的钱。", pplayer->name );

		_getch();
	}
	else{

		pplayer->money -= 100;

		puts( "俺踏上了回家的火车。" );

		_getch();

		collecting_debts( pplayer );

		go_home( pplayer );

		system( "clear" );

		puts( "坐火车一来一回花了俺100元" );

		_getch();
	}
}


//医院
void hospital( player * pplayer ){

	system( "clear" );

	puts( "-医院-\n" );

	if( rand32() % 4 == 0 ){

		puts( "医生：你这个病是癌症，我建议你开刀配合放化疗。" );

		_getch();

		int exit_flag = 0;

		while( 0 == exit_flag ){

			system( "clear" );

			puts( "-你要开刀吗？-\n" );

			puts( "开刀---1" );
			puts( "不开刀---2" );

			int key = _getch();

			if( key == '1' ){

				system( "clear" );

				if( pplayer->money + pplayer->deposit < 10000 ){

					puts( "医生：没钱？没钱不给开刀。" );

					exit_flag = 1;

					_getch();
				}
				else{

					puts( "医生：那我立马为你准备手术。" );

					_getch();

					if( rand32() % 3 == 0 ){

						puts( "因为医疗事故，你直接死在手术台上。" );

						_getch();

						system( "clear" );

						puts( "游戏结束，请按任意键退出程序。" );

						_getch();

						exit( 0 );
					}

					puts( "治疗癌症花光了你所有积蓄。" );

					_getch();

					puts( "你感觉身体比以前更差了。" );

					_getch();

					pplayer->money = 0;
					pplayer->deposit = 0;

					pplayer->hp -= 65;

					if( pplayer->hp <= 0 ){

						puts( "不久之后，你就去世了。" );

						_getch();

						system( "clear" );

						puts( "游戏结束，请按任意键退出程序。" );

						_getch();

						exit( 0 );
					}

					exit_flag = 1;
				}
			}
			else if( key == '2' ){

				system( "clear" );

				puts( "医生：无知，不开刀就等死吧。" );

				_getch();

				exit_flag = 1;
			}
		}
	}
	else{

		printf( "医生：恢复一个健康点只收3000元红包。你要恢复多少？\n：" );

		int num;

		scanf( "%d", &num );

		if( num <= 0 ){

			puts( "医生：不看病的就滚远点，你这不是耽误其他患者吗？下一个。" );

			_getch();
		}
		else{

			if( num > 100 ){

				num = 100;
			}

			int hf = num * 3000;
			
			if( hf > pplayer->money ){

				puts( "医生：没钱看什么病，你这不是耽误其他患者吗？下一个。" );

				_getch();
			}
			else{

				pplayer->money -= hf;
				pplayer->hp += num;

				if( pplayer->hp > 100 ){

					pplayer->hp = 100;
				}

				puts( "医生：好了，下一个。" );

				_getch();
			}
		}
	}
}


//慈善机构
void chishan( player * pplayer ){

	system( "clear" );

	puts( "-慈善机构-\n" );

	printf( "经理：本慈善机构专门帮助富人炒做名声，废话少说，要多少点名声（每点4000元）？\n：" );

	int num;

	scanf( "%d", &num );

	if( num <= 0 ){

		puts( "经理：这个不行，不行，你走吧。" );

		_getch();
	}
	else{

		if( num > 100 ){

			num = 100;
		}

		int hf = num * 4000;

		if( pplayer->money < hf ){

			puts( "经理：没钱？没钱做什么慈善，傻逼。" );

			_getch();
		}
		else{

			pplayer->money -= hf;

			pplayer->rp += num;

			if( pplayer->rp > 100 ){

				pplayer->rp = 100;
			}

			puts( "经理：好了，我们马上就会组织媒体。" );

			_getch();
		}
	}
}


//游戏主程序
void game_start( player iplayer ){

	int exit_flag = 0;

	while( 0 == exit_flag ){

		system( "clear" );


		printf( "%s的状态：在北京混了 %d/60 天\n\n", iplayer.name, iplayer.ndays );

		printf( "现金：%d\t\t存款：%d\t\t欠债：%d\n\n", iplayer.money, iplayer.deposit, iplayer.debt );
		printf( "健康：%d\t\t名声：%d\n\n\n", iplayer.hp, iplayer.rp );
		

		if( 0 == iplayer.location ){

			puts( "北京市地面示意图：\n" );

			puts( "     我要进地铁---Q        海淀街---1    亚运村---2    三元西---3\n" );
			puts( "八角西---4    翠微路---5   府右街---6    ==========    永安里---7\n" );
			puts( "       老板键---W          玉泉营---8    永定门---9    万庄镇---0\n\n" );
		}
		else{

			puts( "北京市地铁示意图：\n" );

			puts( "     我要逛京城---Q        西直门---1    积水潭---2    东直门---3\n" );
			puts( "苹果园---4    公主坟---5   复兴门---6    ==========    建国门---7\n" );
			puts( "       老板键---W          长椿街---8    崇文门---9    北京站---0\n\n" );
		}


		puts( "北京市机构：\n" );

		puts( "银行---A\t医院---B\t吃善机构---C\t仓库---D\n" );
		puts( "北京站---E\t黑中介---F\t网吧---G\t街角---H\n" );

		puts( "退出---Esc" );

		int key = _getch();

		switch( key ){

			case 'q':{

				iplayer.location = ~iplayer.location;
					 }break;

			case 'w':{

					 }break;

			case '1':;
			case '2':;
			case '3':;
			case '4':;
			case '5':;
			case '6':;
			case '7':;
			case '8':;
			case '9':;
			case '0':{

				black_market( &iplayer );
				iplayer.ndays++;
				iplayer.debt = ( int )( iplayer.debt * 1.06 );
					 }break;

			case 'a':{

				bank_work( &iplayer );
					 }break;

			case 'b':{

				hospital( &iplayer );
					 }break;

			case 'c':{

				chishan( &iplayer );
					 }break;

			case 'd':{

					 }break;

			case 'e':{

				beijing_station( &iplayer );

				iplayer.ndays++;

				iplayer.debt = ( int )( iplayer.debt * 1.06 );
					 }break;

			case 'f':{
	
					 }break;

			case 'g':{

				internet_bar( &iplayer );
					 }break;

			case 'h':{

				hehe( &iplayer );
					 }break;

			case 27:{

				if( exit_ask() == 1 ){

					exit_flag = 1;
				}
					}break;
		}


		system( "clear" );

		if( iplayer.hp < 20 ){

			if( iplayer.hp < 1 ){

				printf( "%s因为操劳过度暴死在北京街头。\n", iplayer.name );
				
				_getch();

				system( "clear" );

				puts( "游戏结束，请按任意键退出程序。" );

				_getch();

				exit( 0 );		
			}
			else{

				puts( "俺最近总是感觉头昏脑涨。" );

				_getch();
			}
		}


		if( iplayer.rp < 20 ){

			
			if( iplayer.rp < 1 ){

				puts( "你正在自己的出租屋里数货物，突然门被撞开了，几个警察把你按到。" );

				_getch();

				puts( "走私贩卖假冒伪劣禁品，证据确凿，你的下辈子只好在牢里度过了。" );

				_getch();

				system( "clear" );

				puts( "游戏结束，请按任意键退出程序。" );

				_getch();

				exit( 0 );
			}
			else{

				puts( "因为丧尽天良，北京市工商部门已经盯上了你。" );

				_getch();
			}
		}

		if( iplayer.ndays > 52 ){

			if( iplayer.ndays > 60 ){

				int num = iplayer.money + iplayer.deposit - iplayer.debt;

				puts( "两个月过去了。\n" );

				_getch();

				if( num < 0 ){

					puts( "你依然没有摆脱高利贷，或许最开始就错了，穷人命该如此。" );
				}
				else if( num < 2000 ){

					puts( "你历尽千辛万苦，终于摆脱了高利贷，两手空空回了家。" );
				}
				else if( num < 10000 ){

					puts( "你摆脱了高利贷，也赚了一些零花钱。" );
				}
				else if( num < 100000 ){

					puts( "哈哈，你摆脱高利贷，也挣够了回家娶媳妇的钱。" );
				}
				else if( num < 1000000 ){

					puts( "你摆脱了高利贷，回村过上了富裕的农民生活。" );
				}
				else if ( num < 10000000 ){

					puts( "你摆脱了高利贷，成为了村里的百万富翁。" );
				}
				else{

					puts( "你摆脱了高利贷，但你已经不希望继续过农民的生活了，也许你天生属于商界。" );
				}

				_getch();

				system( "clear" );

				puts( "游戏结束，请按任意键退出程序。" );

				_getch();

				exit( 0 );
			}
			else if( iplayer.ndays == 52 ){

				puts( "还有最后一个星期，加油吧。" );

				_getch();
			}
		}
	}
}







//游戏主菜单
void main_menu(){

	int exit_flag = 0;

	while( 0 == exit_flag ){

		system( "clear" );

		puts( "-北京浮生记-\n" );

		puts( "新的游戏---1" );
		puts( "旧的回忆---2" );
		puts( "开发人员---3" );
		puts( "退出游戏---4" );

		int key = _getch();

		switch( key ){

			case '1':{

				game_start( new_game() );
					 }break;

			case '2':{

				data_mngmnt( NULL );
					 }break;

			case '3':{

				game_author();
					 }break;

			case '4':{
				
				if( exit_ask() == 1 ){

					exit_flag = 1;
				}
					 }break;
		}
	}

	system( "clear" );

	puts( "游戏结束，请按任意键退出程序。" );

	_getch();
}


//主程序
int main( int argc, char * argv[] ){

	system( "title 北京浮生记" );

	srand( ( unsigned int )time( NULL ) );

	main_menu();

	return 0;
}

