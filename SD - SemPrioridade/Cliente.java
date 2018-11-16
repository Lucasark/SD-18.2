import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Cliente {
	private Cliente() {}
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		
		System.out.println("Deseja Ler(L) ou Escrever(E)?\n L ou E ?");
		String operacao = args[0]; //in.nextLine();
		System.out.println("Qual o arquivo destino?");
		String arqDestino = args[1]; //in.nextLine();
		
		try {
				Registry registry = LocateRegistry.getRegistry(null);
				ArqInterface stub = (ArqInterface) registry.lookup("ArqInterface");
				String resposta="";
				if(operacao.equals("L") || operacao.equals("l")) {
					resposta = stub.leituraArquivo(arqDestino);
					System.out.println("resposta: " + resposta);
				}else if(operacao.equals("E") || operacao.equals("e")) {
					System.out.println("O que quer escrever?");
					String input = "O barao deve morrer"; //in.nextLine();
					stub.escritaArquivo(arqDestino,input);
					System.out.println("Escrita concluída");
				}else {
					System.err.println("ERRO : " + operacao);
				}
			} catch (Exception e) {
				System.err.println("Capturando a exceção no Cliente: " + e.toString());
				e.printStackTrace();
			}
	}
}
