#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/mii.h>
#include <linux/sockios.h>
static struct ifreq ifr;
static int phyfd=-1;
void phyopen( char *s, int phyId)
{
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, s); //set to whatever your ethernet device is

    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    mii->phy_id = phyId; //set to your phy's ID
    phyfd = socket(AF_INET, SOCK_DGRAM, 0);
}

int phyread( int reg)
{
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    mii->reg_num = reg; //the register you want to read
    mii->val_in = 0;
    mii->val_out = 0;
    int err = ioctl(phyfd, SIOCGMIIREG, &ifr);
   
    return mii->val_out;
}

void phywrite( int reg, int val)
{
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    mii->reg_num = reg; //the register you want to read
    mii->val_in = val;
    mii->val_out = 0;

    int err = ioctl(phyfd, SIOCSMIIREG, &ifr);
    printf("phywrite %d: reg=0x%02x, val_in=0x%04x, val_out=0x%04x\n", mii->phy_id, mii->reg_num, mii->val_in, mii->val_out);
}

void phyclose()
{
    close(phyfd);
    phyfd=-1;
}


static int atoh( const char *String)
{
    int i, Value = 0, Digit;
    char c;

    for (i=0; c=String[i];i++ ) {
        if (c >= '0' && c <= '9')
            Digit = (int ) (c - '0');
        else if (c >= 'a' && c <= 'f')
            Digit = (int ) (c - 'a') + 10;
        else if (c >= 'A' && c <= 'F')
            Digit = (int) (c - 'A') + 10;
        else
            break;
        Value = (Value << 4) + Digit;
    }

    return Value;
}

#define WITH_MAIN 1

#ifdef WITH_MAIN
void interactive_mode(void){
  int i,wval;
  int phyaddr, reg;
  char cmd[10];
  for (i=0;;i++){
      scanf("%s %x %x %x", cmd, &phyaddr, &reg, &wval);
      if (cmd[0]=='r') {
        phyopen("eth0", phyaddr); 
        printf("%s %x %x %x\n", cmd, phyaddr, reg, phyread( reg));
      }else if ( cmd[0]=='w') {
        phyopen("eth0", phyaddr); 
        phywrite( reg, wval);
        printf("%s %x %x %x\n", cmd, phyaddr, reg, wval);
      }
      phyclose();
  }
}

int main( int argc, char *argv[])
{
    int phyid, reg, i, valout;
    if (!(argc>=3 || (argc==2 && strcmp(argv[1], "int")==0))){
        fprintf(stderr, "Usage: %s <phyaddr> <reg> [writeval]\n", argv[0]);
        return -1; 
    }
    if (argc == 2){
       interactive_mode();
    }
    phyid = atoh(argv[1]);
    reg = atoh(argv[2]);
    /* use eth0 whatever since all phys are connected on the single MDIO controller associated with eth0*/
    phyopen( "eth0", phyid );
    if (argc<4) {
        valout = phyread( reg);
        printf("phyread %d: reg=0x%02x, val_out=0x%04x\n", phyid, reg, valout);
    }else {
        phywrite( reg, atoh(argv[3]));
    }
    
    return 0;
}
#endif
