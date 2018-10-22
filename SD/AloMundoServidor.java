import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.io.*; 

public class AloMundoServidor implements AloMundo {
	private static int count, fila = 0;
	private static boolean lock;
	
	public AloMundoServidor() {}

	public String digaAloMundo() throws Exception {
		System.out.println("Chamada de aplicacao Cliente recebida! " + count);
		/*fila++;
		if(lock){
			try { 
				wait();
			} catch (InterruptedException e)  {
				Thread.currentThread().interrupt(); 
				//Log.error("Thread interrupted", e); 
			}
		}
		lock = true;
		*/
		File file = new File("test.txt"); 
		
		BufferedReader reader = new BufferedReader(new FileReader(file)); 

		String text = "", line; 
		while ((line = reader.readLine()) != null) {
			text = text + "\n" + line; 
		} 
		
		count++;
		Thread.sleep(5000);
		
		// fila--;
		// notify();
		// if(fila==0) {	
			// lock = false;
		// }		
		
		return text;
	}

	public static void main(String args[]) {
		count = 0;
		lock = false;
		try {
			AloMundoServidor obj = new AloMundoServidor();
			AloMundo stub = (AloMundo) UnicastRemoteObject.exportObject(obj, 0);
			Registry registry = LocateRegistry.getRegistry();
			registry.bind("AloMundo", stub);
			System.out.println("Servidor pronto!");
		} catch (Exception e) {
			System.err.println("Capturando excecao no Servidor: " + e.toString());
			e.printStackTrace();
		}
	}
}