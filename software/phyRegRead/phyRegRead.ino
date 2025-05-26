
#include <stdio.h>                                                                   
#include <stdlib.h>                                                                  
#include <stdint.h> 

#define MDCIP PB7 // wiringPi PIN for MDCIP                                            
#define MDIO  PB6 // wiringPi PIN for MDIO      

#define MAXARGS 4

String cmds[MAXARGS] = {"\0"}; // 分割された文字列を格納する配列 

int split(String data, char delimiter, String *dst){
    int index = 0; 
    int datalength = data.length();

    dst[0] = "";
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            dst[index] = "";
        }
        else dst[index] += tmp;
    }
    
    return (index + 1);
}

uint16_t reg_val = 0;
uint8_t reg = 0;

int smi_init(void)                                                                
{                                                                                    
    pinMode(MDCIP, OUTPUT);
    pinMode(MDIO, OUTPUT);
    return 0;
}                                                                                    
                                                                                                                                                                          
/*! Generates a rising edge pulse on IP175 MDC */                                    
void pulse_mdcip(void)                                                               
{                                                                                    
    digitalWrite(MDCIP, 0);                                                          
    delay(1);                                                                       
    digitalWrite(MDCIP, 1);                                                          
    delay(1);                                                                       
}                                                                                    

/*! Performs a smi write */                                                          
void write_smi(uint8_t phy, uint8_t reg, uint16_t data)                              
{                                                                                    
    uint8_t byte;                                                                    
    uint16_t word;                                                                   
                                                                                     
    void (*pulse_mdc)(void);                                                         
    pulse_mdc = pulse_mdcip;                                                         
                                                                                     
    /* MDIO pin is output */                                                         
    pinMode(MDIO, OUTPUT);                                                           
                                                                                     
    digitalWrite(MDIO, 1);                                                           
    for (byte = 0;byte < 32; byte++)                                                 
        pulse_mdc();                                                                 
                                                                                     
    /* Stat code */                                                                  
    digitalWrite(MDIO, 0);                                                           
    pulse_mdc();                                                                     
    digitalWrite(MDIO, 1);                                                           
    pulse_mdc();                                                                     
                                                                                     
    /* Write OP Code */                                                              
    digitalWrite(MDIO, 0);                                                           
    pulse_mdc();                                                                     
    digitalWrite(MDIO, 1);                                                           
    pulse_mdc();                                                                     
                                                                                     
    /* PHY address - 5 bits */                                                       
    for (byte=0x10;byte!=0;byte=byte>>1){                                            
        if (byte & phy)                                                              
            digitalWrite(MDIO, 1);                                                   
        else                                                                         
            digitalWrite(MDIO, 0);                                                   
        pulse_mdc();                                                                 
    }                                                                                
    /* REG address - 5 bits */                                                       
    for (byte=0x10;byte!=0;byte=byte>>1){                                            
        if (byte & reg)                                                              
            digitalWrite(MDIO, 1);                                                   
        else                                                                         
            digitalWrite(MDIO, 0);                                                   
                                                                                     
        pulse_mdc();                                                                 
    }                                                                                
    /* Turn around bits */                                                           
    digitalWrite(MDIO, 1);                                                           
    pulse_mdc();                                                                     
    digitalWrite(MDIO, 0);                                                           
    pulse_mdc();                                                                     
                                                                                     
    /* Data - 16 bits */                                                             
    for(word=0x8000;word!=0;word=word>>1){                                           
        if (word & data)                                                             
            digitalWrite(MDIO, 1);                                                   
        else                                                                         
            digitalWrite(MDIO, 0);                                                   
                                                                                     
        pulse_mdc();                                                                 
    }
                                                                                     
    /* This is needed for some reason... */                                          
    pulse_mdc();                                                                     
    /* Stay in 0 state */                                                            
//  MDCIP = 0;                                                                       
}

uint16_t read_smi(uint8_t phy, uint8_t reg)                                          
{                                                                                    
    uint8_t byte;                                                                    
    uint16_t word, data;                                                             
                                                                                     
    void (*pulse_mdc)(void);                                                         
                                                                                     
    pulse_mdc = pulse_mdcip;                                                         
                                                                                     
    /* MDIO pin is output */                                                         
    pinMode(MDIO, OUTPUT);                                                           
                                                                                     
    digitalWrite(MDIO, 1);                                                           
    for (byte = 0;byte < 32; byte++)                                                 
        pulse_mdc();                                                                 
                                                                                     
    /* Stat code */                                                                  
    digitalWrite(MDIO, 0);                                                           
    pulse_mdc();                                                                     
    digitalWrite(MDIO, 1);                                                           
    pulse_mdc();                                                                     
                                                                                     
    /* Read OP Code */                                                               
    digitalWrite(MDIO, 1);                                                           
    pulse_mdc();                                                                     
    digitalWrite(MDIO, 0);                                                           
    pulse_mdc();                                                                     
                                                                                     
    /* PHY address - 5 bits */                                                       
    for (byte=0x10;byte!=0;){                                                        
        if (byte & phy)                                                              
            digitalWrite(MDIO, 1);                                                   
        else                                                                         
            digitalWrite(MDIO, 0);                                                   
                                                                                     
        pulse_mdc();                                                                 
                                                                                     
        byte=byte>>1;                                                                
    }                                                                                
    /* REG address - 5 bits */                                                       
    for (byte=0x10;byte!=0;){                                                        
        if (byte & reg)                                                              
            digitalWrite(MDIO, 1);                                                   
        else                                                                         
            digitalWrite(MDIO, 0);                                                   
                                                                                     
        pulse_mdc();                                                                 
                                                                                     
        byte=byte>>1;                                                                
    }                                                                                
    /* Turn around bits */                                                           
    /* MDIO now is input */                                                          
    pinMode(MDIO, INPUT);                                                            
    pulse_mdc();                                                                     
    pulse_mdc();                                                                     
    /* Data - 16 bits */                                                             
    data = 0;                                                                        
    for(word=0x8000;word!=0;){                                                       
                                                                                     
        if (digitalRead(MDIO))                                                       
            data |= word;                                                            
                                                                                     
        pulse_mdc();
                                                                         
                                                                                     
        word=word>>1;                                                                
    }                                                                                
                                                                                     
    /* This is needed for some reason... */                                          
    pulse_mdc();                                                                     
    /* Stay in 0 state */                                                            
//  MDCIP = 0;                                                                       
                                                                                     
    return data;                                                                     
}

uint16_t print_read_smi(uint8_t phy, uint8_t reg)
{                                                                                    
  reg_val = read_smi(phy, reg);
  Serial.print("phy=");
  Serial.print(phy, HEX);
  Serial.print("\treg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
}

void read_registers() {
  //TJA1011A
  reg = 0;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 1;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 2;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 3;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 15;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 16;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 17;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 18;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 19;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 20;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 21;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 22;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 23;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 24;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 25;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 26;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 27;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 28;
  reg_val = read_smi(0x06, reg);
  Serial.print("phy=0x06\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);

  //SR8201F
  reg = 0;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 1;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 2;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 3;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 4;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 5;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
  reg = 6;
  reg_val = read_smi(0x02, reg);
  Serial.print("phy=0x02\t");
  Serial.print("reg=");
  Serial.print(reg, DEC);
  Serial.print("\treg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n"); 
  delay(10);
}

void setup()
{
  Serial.begin(115200);

  //for STM32
  while(!Serial.isConnected())
    delay(10);
  
  smi_init();
  
  Serial.print("\n");

  Serial.print("Config Start:");
  Serial.print("\n");
}


void loop()
{
  while (Serial.available() == 0) {}   //wait for data available
  String input = Serial.readString();  //read until timeout
  input.trim();
  
  Serial.println(input);
  int index = split(input, ' ', cmds);

  int args[MAXARGS];
  /*for(int i = 0; i < index; i++){
    Serial.println(cmds[i]);
  }*/
  if (!cmds[0].compareTo("a")) {
    read_registers();
  }
  if (!cmds[0].compareTo("r")) {
    args[1] = cmds[1].toInt();
    args[2] = cmds[2].toInt();
    print_read_smi(args[1], args[2]);
  }
  if (!cmds[0].compareTo("w")) {
    args[1] = cmds[1].toInt();
    args[2] = cmds[2].toInt();
    args[3] = cmds[3].toInt();
    write_smi(args[1], args[2], args[3]);
  }
  
}