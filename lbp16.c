
#include <pci/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "lbp16.h"

#ifdef __linux__
    extern int sd;
    extern socklen_t len;
#elif _WIN32
    extern SOCKET sd;
    extern int len;
#endif
extern struct sockaddr_in server_addr, client_addr;

int lbp16_read(u16 cmd, u32 addr, void *buffer, int size) {
    lbp16_cmd_addr packet;
    int send, recv;
    u8 local_buff[size];

    LBP16_INIT_PACKET4(packet, cmd, addr);
    if (LBP16_SENDRECV_DEBUG)
        printf("SEND: %02X %02X %02X %02X\n", packet.cmd_hi, packet.cmd_lo, packet.addr_hi, packet.addr_lo);
    send = sendto(sd, (char*) &packet, sizeof(packet), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    recv = recvfrom(sd, (char*) &local_buff, sizeof(local_buff), 0, (struct sockaddr *) &client_addr, &len);
    if (LBP16_SENDRECV_DEBUG)
        printf("RECV: %d bytes\n", recv);
    memcpy(buffer, local_buff, size);

    return 0;
}

int lbp16_hm2_read(u32 addr, void *buffer, int size) {
    return lbp16_read(CMD_READ_HOSTMOT2_ADDR32_INCR(size), addr, buffer, size);
}

int lbp16_hm2_write(u32 addr, void *buffer, int size) {
    return 0;
}
