#include <stdio.h>

#include <string.h>

#include <string>

 

static bool GetDestFromUri(char *_tag,char *_uri,std::string &_key,std::string &_session)

{

    char *start_token  = 0 ;
    char *end_token  = 0 ;
    char  tmp[65] ;
    int   offset ;

 
    printf("[%p]\n[%p]\n",tmp,&offset);

    start_token = strstr(_uri,_tag) ;

 

    if ( !start_token ) return false ;

 

    start_token += strlen(_tag) ;

 

    printf("check1 start-token [%s]\n",start_token) ;

 

    end_token = strchr(start_token,'-') ;

    if ( !end_token ) return false ;

 

    printf("check2 end-token [%s]\n",end_token) ;

 

    offset = end_token - start_token ;

    if ( offset <= 0 ) return false ;

 

    printf("check3 offset [%d]\n",offset) ;

 
  //snprintf(tmp,sizeof tmp,"%.*s",offset,start_token) ;
    strncpy(tmp,start_token,offset) ;
    tmp[offset] = 0x00;
    tmp[sizof(tmp) - 1] = 0x00;

    printf("check3 tmp [%s]\n",tmp) ;

 

   _key = tmp ;

start_token = end_token + 1 ;

    printf("check4 start_token[%s]\n",start_token) ;

    end_token   = strchr(start_token,'/') ;

    if ( !end_token )

    {

        printf("error!..\n") ;

        end_token = _uri+strlen(_uri) ;

    }

    printf("check5 end_token [%s]\n",end_token) ;

 

    offset = end_token - start_token ;

    printf("check6 offset [%d]\n",offset) ;

    if ( offset <= 0 ) return false ;

 

    strncpy(tmp,start_token,offset) ;
    tmp[offset] = 0x00;
  //snprintf(tmp,sizeof tmp,"%.*s",offset,start_token) ;
    printf("check7 offset[%d]\n",offset);
    printf("[%s]\n",tmp);

 

    _session = tmp ;

 

    return true ;

}

 

int main(int argc,char **argv)

{

    char tag[255] ;

    char uri[255] ;

 

    strcpy(tag,"/nudr-gxp-notify/gxpa/") ;

  //strcpy(uri,"/npcf-event-exposure/v1/nudr-gxp-notify/gxpa/56035705000-01-4694-23-d1-DNSN-vSPGW222.skt.net;1663078720;12008753;63209140-450050867066239/notify") ;
    strcpy(uri,argv[1]);

 

    std::string key,session ;

 

 

    printf("tag = [%s]\n",tag) ;

    printf("uri = [%s]\n",uri) ;

    for(int i =0 ; i< 100; ++i)
    {
        GetDestFromUri(tag,uri,key,session) ;
        printf("key=%s,session=%s\n",key.c_str(),session.c_str()) ;
    }

 


}
