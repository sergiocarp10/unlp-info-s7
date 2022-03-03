-- Alumno: CALDERON Sergio Leandro
-- Legajo: 02285/4

-- Utilizaci�n del Paquete Utils (pr�ctica 8)
use work.Utils.all;

-- Entidad de Testbench, no requiere entradas ni salidas
entity TestMultiplicador is end;
	
-- Arquitectura del Testbench
architecture Driver of TestMultiplicador is	

	-- Componente sobre el cual se realizar�n las pruebas
	component Multiplicador
	port(									 
		STB, CLK: in Bit; 
		A, B: in Bit_Vector(3 downto 0); 
		Result: out Bit_Vector(7 downto 0);
		Done: out Bit
	);
	end component;


	-- Se�ales de entrada y salida del Multiplicador
	signal A, B: Bit_Vector(3 downto 0);	
	signal Result: Bit_Vector(7 downto 0);
	signal STB, CLK, Done: Bit; 
	
	-- Se�al auxiliar para verificar cada operaci�n
	signal resultado: NATURAL := 0;

begin
	-- Se instancia el multiplicador
	Multi: Multiplicador port map(STB, CLK, A, B, Result, Done);

	-- Se asigna el per�odo del reloj
	Clock(CLK,5 ns,5 ns); 
	
	Stimulus : process
	begin		
		
		-- Se realiza un test exhaustivo (todas las combinaciones de A y B)
		-- Por cada valor de A
		for i in 0 to 15 loop				   			
			-- Se convierte el primer operando a binario
			A <= Convert(i, 4);				
			
			-- Por cada valor de B
			for j in 0 to 15 loop
				-- Se convierte el segundo operando a binario
				B <= Convert(j, 4);	
				
				-- Se da comienzo a la operaci�n  
				wait until CLK'Event and CLK = '1';
 				STB <= '1';
				wait until CLK'Event and CLK = '0';
				STB <= '0';	
				
				-- Se espera al fin de la operaci�n	
				wait until Done = '1';	 
			
				-- Se verifica el resultado obtenido
				assert Result = Convert(resultado, 8) report "Resultado incorrecto" severity FAILURE;
				wait for 5 ns;
				
				-- Se actualiza el resultado esperado
				resultado <= resultado + i;
				
			end loop;
			
			-- Se reinicia el resultado	   
			resultado <= 0;
			
		end loop;
		
		-- Terminar simulaci�n (usando esta configuraci�n tarda aprox 28500 ns)
		wait;
	end process;

end;
