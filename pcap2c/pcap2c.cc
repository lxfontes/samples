/*
 * Author: lxfontes
 *
 * there's no argv parser for this guy, sorry.
 * This will parse a pcap file based on source and destination
 * mac addresses, so it can be loaded by a traffic gen.
 * Usage is:
 * ./pcap2c pcapfile source_ether_addr dst_ether_addr
 * Example:
 *
 * ./pcap2c http.cap 00:1b:ed:7c:dc:63 00:09:35:03:3c:00
 *
 */

#include <iostream>
#define __FAVOR_BSD
#include <pcap/pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <sys/socket.h>
#ifdef __linux__
#include <netinet/ether.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <list>
#include <map>
#include <getopt.h>


#define COLS 30

struct arguments_conf{
  std::string filename;
  struct ether_addr client_side;
  struct ether_addr server_side;
  unsigned long pkt_count;
  std::list<bool> pkt_direction;
  std::list<uint32_t> pkt_size;



  arguments_conf():
    pkt_count(0){
      //nothing to see here, move along
    }
};


int processFile(arguments_conf &conf){
  pcap_t *readFile;
  char errBuf[PCAP_ERRBUF_SIZE];

  readFile = pcap_open_offline(conf.filename.c_str(), errBuf);

  const u_char *packet;
  struct pcap_pkthdr pkt_header;

  printf("#include \"packets.h\"\n");
  printf("#include <stdint.h>\n");
  while((packet = pcap_next(readFile,&pkt_header))){
    struct ether_header *pkt_ether = (struct ether_header *)packet;
    bool from_client = false;
    //only deal with ethernet frames
    if(ntohs(pkt_ether->ether_type) != ETHERTYPE_IP){
      continue;
    }

    //pcap filter prevents this to get non client/server eth packets
    if(memcmp(pkt_ether->ether_shost,
              &conf.client_side,
              sizeof(struct ether_addr)) == 0){
      from_client = true;
    }

    printf("void load_pkt_%ld()  {\n    unsigned char pkt[] = {\n    ",conf.pkt_count);
    for(unsigned int i=0;i<pkt_header.caplen;i++){
      printf("0x%02X", packet[i]);
      if(i != (pkt_header.caplen - 1)){
        printf(",");
      }
     if((i + 1) % COLS == 0){
       printf("\n    ");
     }
    }
    printf("\n    };\n");
    printf("    uint32_t pkt_len = %d;\n",pkt_header.caplen);
    printf("    uint8_t pkt_client = %d;\n",(from_client==true?1:0));
    printf("    load_packet(pkt,pkt_len,pkt_client);\n");
    printf("  }\n");
    conf.pkt_count++;
  }
  printf("extern \"C\" void load_captured_packets(){\n");
  for(unsigned long i=0;i<conf.pkt_count;i++){
    printf("  load_pkt_%ld();\n",i);
  }
  printf("}\n");
  return(0);
}

int main(int argc,char **argv){
  arguments_conf conf;
  int result;
  struct ether_addr *etmp;

  conf.filename.assign(argv[1]);

  //client
  etmp = ether_aton(argv[2]);
  memcpy(&conf.client_side,etmp,sizeof(conf.client_side));

  //server
  etmp = ether_aton(argv[3]);
  memcpy(&conf.server_side,etmp,sizeof(conf.server_side));


  result = processFile(conf);

  return(result);
}
