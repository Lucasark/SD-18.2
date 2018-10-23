import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Cliente {
	private Cliente() {}
	public static void main(String[] args) {
		String host = null;
		
		String operacao = args[0];
		String arqDestino = args[1];
		
		try {
				Registry registry = LocateRegistry.getRegistry(host);
				Interface stub = (Interface) registry.lookup("Interface");
				String resposta="";
				if(operacao.equals("L") || operacao.equals("l")) {
					resposta = stub.lerArquivo(arqDestino);
					System.out.println("resposta: " + resposta);
				}else if(operacao.equals("E") || operacao.equals("e")) {
					String input = args[2];
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
