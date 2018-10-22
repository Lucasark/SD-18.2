import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Callable;

public class ThreadEscrita implements Callable<String>{

  //protected BlockingQueue<String> blockingQueue = null;
  protected String nomeArq = "";
  protected String input = "";
  protected String resultado = "Ocorreu um erro";

  public ThreadEscrita(BlockingQueue<String> blockingQueue,String nomeArq,String input){
    //this.blockingQueue = blockingQueue;    
    this.nomeArq = nomeArq;
    this.input = input;
  }

  public ThreadEscrita(String nomeArq,String input){
    this.nomeArq = nomeArq;
    this.input = input;
  }

  @Override
  public String call() throws Exception {
    PrintWriter writer = null;

    try {

        writer = new PrintWriter(new FileWriter(nomeArq,true));


        /*
        while(true){
            String buffer = blockingQueue.take();
            //Check whether end of file has been reached
            if(buffer.equals("EOF")){ 
                break;
            }
            writer.println(buffer);
        } */              

        writer.println(input);

        resultado = "Escrito com sucesso >:D em : " + nomeArq;

    } catch (FileNotFoundException e) {
        resultado = "Ocorreu um erro";
        e.printStackTrace();

    }finally{
        writer.close();
    } 


    return resultado;
  }

}