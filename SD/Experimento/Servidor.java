import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.io.*;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue; 
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;


public class Servidor implements Interface {
	private static int count, fila = 0;
	private static boolean lock;
	public BlockingQueue<String> blockingQueue = null;
	public ExecutorService executor;
	
	public Servidor(BlockingQueue<String> blockingQueueR, ExecutorService executorR) {
		this.blockingQueue = blockingQueueR;
		this.executor = executorR;
	}
	
	public String lerArquivo(String nomeArq) throws Exception {
		
		//substituir o parametro para nomeArq quando formos rodar mesmo
		
		System.out.println("Chamada de leitura do Cliente recebida! " + count);

		//String nomeArq = "leitura.txt";  // aqui podiamos pegar input

		Callable<String> callable = new ThreadLeitura(blockingQueue,nomeArq);

		Future<String> future = executor.submit(callable);

		String resposta = "";

		try {
			    resposta = future.get();
			} catch (InterruptedException | ExecutionException e) {
			    e.printStackTrace();
			}

		count++;

		Thread.sleep(5000);

		return resposta;
	}
  
	public String escreverArquivo(String nomeArq,String input) throws Exception {
		
		//substituir o parametro para nomeArq e input quando formos rodar mesmo
		
		System.out.println("Chamada de escrita do Cliente recebida! " + count);

		/*
		Random random = new Random();
        int arqNumb = random.nextInt((5 - 1) + 1) + 1;

		String nomeArq = "escrita" + arqNumb + ".txt";  // aqui podiamos pegar input
		
		String input = "Oi eu sou o cliente " + count + " e quero escrever: INPUT"; // pegar input tambem
		*/

		Callable<String> callable = new ThreadEscrita(blockingQueue,nomeArq,input);

		Future<String> future = executor.submit(callable);

		String resposta = "";

		try {
			    resposta = future.get();
			} catch (InterruptedException | ExecutionException e) {
			    e.printStackTrace();
			}

		count++;

		Thread.sleep(3000);

		return resposta;

	}


	public static void main(String args[]) {
		count = 0;
		lock = false;
		try {

			ExecutorService executor = Executors.newFixedThreadPool(10);
			BlockingQueue<String> queue = new ArrayBlockingQueue<String>(1024);


			Servidor obj = new Servidor(queue,executor);
			Interface stub = (Interface) UnicastRemoteObject.exportObject(obj, 0);
			Registry registry = LocateRegistry.getRegistry();
			registry.bind("Interface", stub);
			System.out.println("Servidor pronto!");
		} catch (Exception e) {
			System.err.println("Capturando excecao no Servidor: " + e.toString());
			e.printStackTrace();
		}
		/*
		finally{
			executorService.shutdown();
			try {
				if (!executorService.awaitTermination(800, TimeUnit.MILLISECONDS)) {
				    executorService.shutdownNow();
				} 
			} catch (InterruptedException e) {
				executorService.shutdownNow();
			}
		}*/
	}

}