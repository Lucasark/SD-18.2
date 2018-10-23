import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Cliente {
	private Cliente() {}
	public static void main(String[] args) {
		String host = (args.length < 1) ? null : args[0];
		
		Scanner in = new Scanner(System.in);
		
		System.out.println("Deseja Ler(L) ou Escrever(E)?\n L ou E ?");
		String operacao = in.nextLine();
		System.out.println("Qual o arquivo destino?");
		String arqDestino = in.nextLine();
		
		try {
				Registry registry = LocateRegistry.getRegistry(host);
				Interface stub = (Interface) registry.lookup("Interface");
				String resposta="";
				if(operacao.equals("L") || operacao.equals("l")) {
					resposta = stub.lerArquivo(arqDestino);
					System.out.println("resposta: " + resposta);
				}else if(operacao.equals("E") || operacao.equals("e")) {
					System.out.println("O que qeur escrever?");
					String input = in.nextLine();
					resposta = stub.escreverArquivo(arqDestino,input);
					System.out.println("resposta: " + resposta);
				}else {
					System.err.println("ERRO : " + operacao);
				}
				 //notar que aqui eh onde tamo dizendo o que chamar
				
			} catch (Exception e) {
				System.err.println("Capturando a excecao no Cliente: " + e.toString());
				e.printStackTrace();
			}
	}
}
