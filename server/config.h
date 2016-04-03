//2-22
#ifndef CONFIG_H
#define CONFIG_H
#define device "em0"
#define MAXBYTES2CAPTURE 2048
#define dump_file "/home/workspace/networkbehavior/tmp"
#define cb_http_a_Size 1333 /* ringbuff a size,用于保存抓到http包  */
#define cb_http_b_Size 18 /* ringbuff b size,用于保存要拦截的http包  */

#define white_list 1
#define person_list 2
#define group_list 3
#define whole_list 4

//定义数据库操作的宏
//system config
#define GET_SUB_NET_CONFIG "select Config_data from system_configure where Config_name ='SUB_NET'"
#define GET_NET_INTERFACE_CONFIG "select Config_data from system_configure where Config_name ='NET_interface'"
#define GET_GATEWAY_CONFIG "select Config_data from system_configure where Config_name ='GATEWAY'"
//#define GET_FACK_GATEWAY_MAC_CONFIG "select Config_data,Config_data1,Config_data2,Config_data3,Config_data4,Config_data5 from system_configure where Config_name ='FACK_GATEWAY_MAC'"
#define GET_FACK_GATEWAY_MAC_CONFIG "select Config_data from system_configure where Config_name ='FACK_GATEWAY_MAC'"

//用于URL
#define GET_URL_white_list "select Detail , User_IP from tactics_allow where Tactics_type != 'app_im' and Tactics_type != 'app_game' and Tactics_type != 'app_other' and Tactics_type != 'app_p2p' and Enable =1"
//#define GET_URL_black_list "select Detail , IP from tactics_allow where Tactics_type != 'P2P' and Tactics_type != 'Game' and Tactics_type != 'IM'"
#define GET_URL_person_list "select Detail , User_IP from tactics_user where Tactics_type != 'app_im' and Tactics_type != 'app_game' and Tactics_type != 'app_other' and Tactics_type != 'app_p2p' and Enable=1"
/*
#define GET_URL_group_list "select tactics_user_group.Detail ,user.IP  from tactics_user_group,user where user.Gid = tactics_user_group.Gid \
and  tactics_user_group.Enable=1 and tactics_user_group.Tactics_type!='IM' and tactics_user_group.Tactics_type!='P2P'  \
and tactics_user_group.Tactics_type!='Game'"
*/
#define GET_URL_group_list "select Detail,Gid from tactics_user_group where Enable =1 and Tactics_type!='app_im' and Tactics_type!='app_p2p'  \
and Tactics_type!='app_game'and Tactics_type!='app_other'"

#define GET_URL_whole_list "select Detail from tactics_global where Tactics_type = 'url' and Enable =1"

#define  GET_URL_kinds_list "select Name from app_table where Tactics_type = 'url' and Name !='all'"
//#define GET_URL_adult_list "select URL from url_adult"
//#define GET_URL_news_list "select URL from url_news"
//#define GET_URL_music_movie_list "select URL from url_music_movie"

//用于pattern
#define GET_TCP_APP_pattern "select Feature,ID,Name from feature_tcp"
#define GET_UDP_APP_pattern "select Feature,ID,Name from feature_udp"

//用于app
#define GET_APP_white_list "select Tactics_type ,Detail , User_IP from tactics_allow where Tactics_type != 'url' and Enable =1"
#define GET_APP_person_list "select Tactics_type ,Detail , User_IP from tactics_user where (Tactics_type = 'app_p2p' or Tactics_type = 'app_game' or Tactics_type = 'app_im' or Tactics_type = 'app_other') and Enable=1"
#define GET_APP_group_list "select Tactics_type ,Detail,Gid from tactics_user_group where Enable =1 and (Tactics_type ='app_im' or Tactics_type ='app_p2p'  \
or Tactics_type ='app_game' or Tactics_type ='app_other')"

#define GET_APP_whole_list "select Tactics_type ,Detail from tactics_global where (Tactics_type = 'app_p2p' or Tactics_type = 'app_game' or Tactics_type = 'app_im' or Tactics_type = 'app_other')and Enable =1"

#define GET_APP_IM_list "select Name from app_table where Tactics_type = 'app_im' and Name !='all'"
#define GET_APP_GAME_list "select Name from app_table where Tactics_type = 'app_game' and Name !='all'"
#define GET_APP_P2P_list "select Name from app_table where Tactics_type = 'app_p2p' and Name !='all'"
#define GET_APP_OTHER_list "select Name app_table where Tactics_type = 'app_other' and Name !='all'"

//用于建立user 哈希表
#define GET_user_gid_list "select User_IP,Gid from user"
#define GET_user_uid_list "select User_IP,Uid from user"
#define GET_user_name_list "select User_IP,Name from user"
/*gcc main.c http_hash_func.c IM_process.c http_pcap.c http_process.c public.c ringbuff.c sql_func.c tcp_block.c usr_func.c arp_spoofind.c audit.c audit_data_callback.c audit_test_callback.c ip_fragment.c list.c spk_pcap.c spk_process.c tcp_cbna_hash.c tcp_process.c tcp_stream_cbna.c udp_block.c udp_process.c -o tttest -lpcap -lnet -lglib-2.0 -lgthread-2.0 -lmysqlclient -lintl -liconv -lpcre -lz -g
*/
/*valgrind --tool=memcheck --leak-check=full --trace-children=yes --log-file=llo --show-possibly-lost=yes --track-origins=yes ./tttest
*/

#endif
