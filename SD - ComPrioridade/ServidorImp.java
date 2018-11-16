import java.io.*; 
import java.util.concurrent.*; 

public class ServidorImp implements ArqInterface {
	static int count, fila;
	
	Semaphore semEscritor;
	Semaphore semLeitor;

	public String leituraArquivo(String fileName) throws Exception {
		
		semLeitor.acquire();
		
		int rank = count++;
		fila++;
		if(fila == 0){
			semEscritor.acquire();
		}
		semLeitor.release();
		
		System.out.println("Chamada de aplicacao Cliente recebida! " + rank);
		File file = new File(fileName); 
		
		BufferedReader reader = new BufferedReader(new FileReader(file)); 

		String text = "", line; 
		while ((line = reader.readLine()) != null) {
			text = text + "\n" + line; 
		}
		
		Thread.sleep(3000);
			
		semLeitor.acquire();
		fila--;
		if(fila == 0){
			semEscritor.release();
		}		
		semLeitor.release();
		
		System.out.println("Leitura de arquivo do cliente " + rank + " efetuada");
		return text;		
	}
	
	public void escritaArquivo(String fileName, String input) throws Exception {
		semEscritor.acquire();
		count++;
		System.out.println("Chamada de aplicacao Cliente escritor recebida! " + count);
		
		BufferedWriter writer = new BufferedWriter(new FileWriter(fileName, true));
		writer.append(count + ": " + input + "\n");
		 
		writer.close();
		
		Thread.sleep(8000);
		
		System.out.println("Escrita de arquivo do cliente " + count + " efetuada");
		semEscritor.release();
	}
}