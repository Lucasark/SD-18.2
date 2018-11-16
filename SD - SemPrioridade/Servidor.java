import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.concurrent.*; 
import java.io.*; 

public class Servidor implements ArqInterface {
	
	static int count, filaL, filaE;
	
	Semaphore semEscritor;
	Semaphore semLeitor;
	Semaphore mtx1, mtx2;
	
	public Servidor() {
		semEscritor = new Semaphore(1); 
		semLeitor = new Semaphore(1); 
		mtx1 = new Semaphore(1); 
		mtx2 = new Semaphore(1); 
		filaL = 0;
		count = 0;
	}

	public String leituraArquivo(String fileName) throws Exception {
		int rank = ++count;
		System.out.println("Chamada de aplicacao Cliente LEITOR recebida! " + rank);
		semLeitor.acquire();
			mtx1.acquire();
				filaL++;
				if(filaL == 1){
					semEscritor.acquire();
				}
			mtx1.release();
		semLeitor.release();
		
		System.out.println("== Operacao de LEITURA " + rank + " iniciada");
		File file = new File(fileName); 
		
		BufferedReader reader = new BufferedReader(new FileReader(file)); 

		String text = "", line; 
		while ((line = reader.readLine()) != null) {
			text = text + "\n" + line; 
		}
		
		Thread.sleep(3000);
		
		System.out.println("LEITURA de arquivo do cliente " + rank + " efetuada");
			
		mtx1.acquire();
			filaL--;
			if(filaL == 0){
				semEscritor.release();
			}		
		mtx1.release();
		
		return text;		
	}
	
	public void escritaArquivo(String fileName, String input) throws Exception {
		int rank = ++count;
		System.out.println("Chamada de aplicacao Cliente ESCRITOR recebida! " + rank);
		
		semLeitor.acquire();
		semEscritor.acquire();
		
		System.out.println("== Operacao de ESCRITA iniciada. Rank:" + rank);
		
		BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, true));
		writer.append(rank + ": " + input + "\n");
		 
		writer.close();
		
		Thread.sleep(8000);
		
		System.out.println("ESCRITA de arquivo do cliente " + rank + " efetuada");
		
		semEscritor.release();
		semLeitor.release();
	}

	public static void main(String args[]) {		
		try {
			Servidor obj = new Servidor();
			
			ArqInterface stub = (ArqInterface) UnicastRemoteObject.exportObject(obj, 0);
			
			Registry registry = LocateRegistry.getRegistry();
			
			registry.bind("ArqInterface", stub);
			System.out.println("Servidor pronto!");
		} catch (Exception e) {
			System.err.println("Capturando excecao no Servidor: " + e.toString());
			e.printStackTrace();
		}
	}
}