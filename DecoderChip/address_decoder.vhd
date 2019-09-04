library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity address_decoder is
Port (
a11 : In std_logic;
a10 : In std_logic;
a9: In std_logic;
a8  : In std_logic;

ram_cs: Out std_logic;
adc_cs : Out std_logic;
oled_cs: Out std_logic;
oled_dc: Out std_logic --Not necessary...
);

attribute LOC : string;
attribute LOC of ram_cs: signal is "P19";
attribute LOC of adc_cs: signal is "P18";
attribute LOC of oled_cs: signal is "P17";
attribute LOC of oled_dc: signal is "P16";--Not necessary...

attribute LOC of a11     : signal is "P1";
attribute LOC of a10     : signal is "P2";
attribute LOC of a9      : signal is "P3";
attribute LOC of a8      : signal is "P4";
end address_decoder;

architecture behave of address_decoder is begin
--implement the functionality here

  process(a11, a10, a9, a8)
    begin
        ram_cs<='1';
        adc_cs<='1';
        oled_cs<='1';
        oled_dc<='1';

        if (a11='1')   then --SRAM address
        ram_cs<='0';
        elsif (a10='1') then --ADC address
        adc_cs<='0';
        elsif (a9='1') then --OLED Data address
        oled_dc<='0';
        else --Remaining case: OLED Command address
        oled_cs<='0';
        end if;
  end process;

end behave;
 
