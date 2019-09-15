library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_signed.all;
library work;
use work.mult_2x2_pack.all;

entity tempsFPGA is
	--generic (n : integer :=1023 )
	port(
	clk: in std_logic;
	col: in integer range 0 to n;--on suppose aux plus 1024
	lig: in integer range 0 to n;
	Mat: in t_2d_array;
	G: out t_2d_array;
	);
end tempsFPGA;

architecture archi of tempsFPGA is
	signal Gx:t_2d_array;
	signal Gy:t_2d_array;
	signal i:integer range 0 to n :=1;
	signal j:integer range 0 to n :=1;
	signal k:integer range 0 to n :=0;
	signal l:integer range 0 to n :=0;
begin
	process (clk)
	begin
		if(rising_edge(clk)) then
				if (i<lig) then
						if(j<col) then
								Gx(i)(j)<=	"10000000"
												+"000"&Mat(i-1)(j+1)(7 downto 3)-"000"&Mat(i-1)(j-1)(7 downto 3)
												+"00"&Mat(i)(j+1)(7 downto 2)-"00"&Mat(i)(j-1)(7 downto 2)
												+"000"&Mat(i+1)(j+1)(7 downto 3)-"000"&Mat(i+1)(j+1)(7 downto 3);
												
								Gy(i)(j)<=	"10000000"
												+"000"&Mat(i-1)(j-1)(7 downto 3)-"000"&Mat(i-1)(j-1)(7 downto 3)
												+"00"&Mat(i-1)(j+1)(7 downto 2)-"00"&Mat(i)(j-1)(7 downto 2)
												+"000"&Mat(i-1)(j+1)(7 downto 3)-"000"&Mat(i+1)(j+1)(7 downto 3);
								j<=j+1;
						elsif
								i<=i+1;
								j<=1;
						end if;
				elsif		--sobel fini,on a deux matrice Gx et Gy
						if(k<lig) then
								if(l<col) then
										if	(k=0) then
												G(k)(l)<="00000000";	--le bord de marrice G
										elsif	(l=(col-1))
												G(k)(l)<="00000000";	--le bord de droite
										elsif	(l=0)
												G(k)(l)<="00000000";	--le bord de gauch
										elsif	(k=(lig-1))
												G(k)(l)<="00000000";	--la base du matrice G
										else
												G(k)(l)<="0"&Gx(k)(l)+"0"&Gy(k)(l);
										end if;
										l<=l+1;
								end if;
								k<=k+1;
								l<=0;
						end if;
				end if;
		end if;
	end process;
	
end archi;