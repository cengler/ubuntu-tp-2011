import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;


public class SchedRRTest {

	public static Integer IDLE_TASK = -1;
	
	public static String LOAD = "LOAD";
	public static String CPU = "CPU";
	public static String BLOCK = "BLOCK";
	public static String UNBLOCK = "UNBLOCK";
	public static String EXIT = "EXIT";

	public String[] estados = {LOAD, CPU, BLOCK, UNBLOCK, EXIT};
	private int tiempoInactivo = 0;
	private Map<Integer, Integer> latencia = new HashMap<Integer, Integer>();
	private Map<Integer, Integer> exit = new HashMap<Integer, Integer>();
	private Map<Integer, Integer> load = new HashMap<Integer, Integer>();
	private Map<Integer, Integer> block = new HashMap<Integer, Integer>();

	// TIEMPO BLOQUEADO
	private Map<Integer, Integer> blockTime = new HashMap<Integer, Integer>();
	// EXEC BLOQUEADO
	private Map<Integer, Integer> execTime = new HashMap<Integer, Integer>();

	private Map<Integer, Integer> betweenIO = new HashMap<Integer, Integer>();

	// TIEMPO BLOQUEADO
	private Map<Integer, Integer> betweenIOTime = new HashMap<Integer, Integer>();
	
	public static void main(String[] args) throws FileNotFoundException {
		new SchedRRTest(System.in);
	}

	public SchedRRTest(InputStream is)
	{
		try{
			DataInputStream in = new DataInputStream(is);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String strLine;

			while ((strLine = br.readLine()) != null)   {
				procesarLinea(strLine);
			}
			in.close();
		}catch (Exception e){
			System.err.println("Error: " + e.getMessage());
		}
		
		
		System.out.print("IDLE " + tiempoInactivo + " ");

		for (Integer key : load.keySet()) {
			System.out.print((exit.get(key)-load.get(key)) + " "); // TIEMPO EN EJECUCION
			System.out.print((latencia.get(key)-load.get(key)) + " "); // LATENCIA
			Integer block = blockTime.get(key);
			if(block==null)
				block = 0;
			System.out.print((exit.get(key)-load.get(key)-block-execTime.get(key)) + " "); // WAITING TIME
			System.out.print(betweenIOTime.get(key) + " "); // TIEMPO ENTRE IOs
		}
		
		System.out.print("\n");
	}

	private void procesarLinea(String strLine) throws Exception {

		if( strLine.charAt(0) == '#' )
		{
			return;
		}

		String[] lineaArray = strLine.split(" ");
		if(lineaArray.length != 3)
		{
			return;
		}

		String state = lineaArray[0];
		Integer time = Integer.valueOf(lineaArray[1]);
		Integer pid = Integer.valueOf(lineaArray[2]);

		if(state.equals(LOAD)) 
		{
			load.put(pid, time);
			betweenIO.put(pid, time);
			betweenIOTime.put(pid, 0);
		}
		else if(state.equals(CPU))
		{

			if(pid.equals(IDLE_TASK))
			{
				tiempoInactivo++;
			}
			else
			{
				if(execTime.get(pid) == null)
					execTime.put(pid, 1);
				else 
					execTime.put(pid, execTime.get(pid)+1);
			}
		}
		else if(state.equals(EXIT)) 
		{
			exit.put(pid, time);
			
			int tiempoEntreIOMaximo = betweenIOTime.get(pid);
			int tiempoDesdeUltimoIO = time-betweenIO.get(pid);
			if(tiempoDesdeUltimoIO > tiempoEntreIOMaximo)
				betweenIOTime.put(pid, tiempoDesdeUltimoIO);
			
		}
		else if(state.equals(BLOCK))
		{
			int tiempoEntreIOMaximo = betweenIOTime.get(pid);
			int tiempoDesdeUltimoIO = time-betweenIO.get(pid);
			if(tiempoDesdeUltimoIO > tiempoEntreIOMaximo)
				betweenIOTime.put(pid, tiempoDesdeUltimoIO);
			betweenIO.put(pid, time);

			if(latencia.get(pid) == null)
					latencia.put(pid, time);
			block.put(pid, time);
		}
		else if(state.equals(UNBLOCK))
		{
			int thisBlockTime = time - block.get(pid);
			if( blockTime.get(pid) == null)
				blockTime.put(pid, thisBlockTime);
			else
				blockTime.put(pid, blockTime.get(pid)+thisBlockTime);
		}
	}
}
