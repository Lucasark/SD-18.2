import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Cliente {
	private Cliente() {}
	public static void main(String[] args) {
		String host = (args.length < 1) ? null : args[0];
		try {
				Registry registry = LocateRegistry.getRegistry(host);
				Interface stub = (Interface) registry.lookup("Interface");
				String resposta = stub.escreverArquivo(); //notar que aqui eh onde tamo dizendo o que chamar
				System.out.println("resposta: " + resposta);
			} catch (Exception e) {
				System.err.println("Capturando a exceção no Cliente: " + e.toString());
				e.printStackTrace();
			}
	}
}
