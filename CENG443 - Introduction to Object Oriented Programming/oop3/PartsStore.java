import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.ArrayList;
import hardware.*;
import java.util.Comparator;
import java.util.Optional;
import java.util.stream.Collectors;

public class PartsStore {
    

    private List<CPU> listOfCPUs = new ArrayList<>();
    private List<GPU> listOfGPUs = new ArrayList<>();
    private List<HardDrive> listOfHarddrives = new ArrayList<>();
    private List<Keyboard> listOfKeyboards = new ArrayList<>();
    private List<Memory> listOfMemories = new ArrayList<>();
    private List<Monitor> listOfMonitors = new ArrayList<>();
    private List<Motherboard> listOfMotherboards = new ArrayList<>();
    private List<Mouse> listOfMouses = new ArrayList<>();
    private List<PSU> listOfPSUs = new ArrayList<>();

  public PartsStore(){
    //input here 
    String readFile = "pcparts.csv";
    String readLine = "";
    String splitter = ",";
    try (BufferedReader bufferedReader = new BufferedReader(new FileReader(readFile))) { 
        while ((readLine = bufferedReader.readLine()) != null) {
         // use comma as separator 
         String[] componentData = readLine.split(splitter);
         String componentType = componentData[0];
          switch(componentType){
          case "CPU":{
            String cpuBrand = componentData[1];
            String cpuModel = componentData[2];
            int cpuCoreCount = Integer.parseInt(componentData[3]);
            double cpuClockSpeed = Double.parseDouble(componentData[4].substring(0,componentData[4].length()-3));
            double cpuPrice = Double.parseDouble(componentData[5].split(" ")[0]);
            listOfCPUs.add(new CPU("CPU",cpuBrand,cpuModel,cpuCoreCount,cpuClockSpeed,cpuPrice));
            break;
          }
          case "GPU":{
            String gpuBrand = componentData[1];
            String gpuModel = componentData[2];
            String gpuChipset = componentData[3];
            int gpuCapacity = Integer.parseInt(componentData[4].substring(0,componentData[4].length()-2));
            double gpuClockSpeed = Double.parseDouble(componentData[5].substring(0,componentData[5].length()-3));
            double gpuPrice = Double.parseDouble(componentData[6].split(" ")[0]);
            listOfGPUs.add(new GPU("GPU",gpuBrand,gpuModel,gpuChipset,gpuCapacity,gpuClockSpeed,gpuPrice));
            break;
          }
          case "Hard Drive":{
            String hdBrand = componentData[1];
            String hdModel = componentData[2];
            int hdCapacity = Integer.parseInt(componentData[3].substring(0,componentData[3].length()-2));
            double hdPrice = Double.parseDouble(componentData[4].split(" ")[0]);
            listOfHarddrives.add(new HardDrive("Hard Drive",hdBrand,hdModel,hdCapacity,hdPrice));
            break;
          }
          case "Keyboard":{
            String keyBrand = componentData[1];
            String keyModel = componentData[2];
            String keyConnectionType = componentData[3];
            double keyPrice = Double.parseDouble(componentData[4].split(" ")[0]);
            listOfKeyboards.add(new Keyboard("Keyboard",keyBrand,keyModel,keyConnectionType,keyPrice));
            break;
          }
          case "Memory":{
            String memBrand = componentData[1];
            String memModel = componentData[2];
            String memSocket = componentData[3];
            int memCapacity = Integer.parseInt(componentData[4].substring(0,componentData[4].length()-2));
            double memClockSpeed = Double.parseDouble(componentData[5].substring(0,componentData[5].length()-3));
            double memPrice = Double.parseDouble(componentData[6].split(" ")[0]);
            listOfMemories.add(new Memory("Memory",memBrand,memModel,memSocket,memCapacity,memClockSpeed,memPrice));
            break;
          }
          case "Monitor":{
            String monBrand = componentData[1];
            String monModel = componentData[2];
            String monAspectRatio = componentData[3];
            double monSize = Double.parseDouble(componentData[4]);
            double monPrice = Double.parseDouble(componentData[5].split(" ")[0]);
            listOfMonitors.add(new Monitor("Monitor",monBrand,monModel,monAspectRatio,monSize,monPrice));
            break;
          }
          case "Motherboard":{
            String mothBrand = componentData[1];
            String mothModel = componentData[2];
            String mothSocket = componentData[3];
            int mothRamSlots= Integer.parseInt(componentData[4]);
            double mothPrice = Double.parseDouble(componentData[5].split(" ")[0]);
            listOfMotherboards.add(new Motherboard("Motherboard",mothBrand,mothModel,mothSocket,mothRamSlots,mothPrice));
            break;
          }
          case "Mouse":{
            String mouBrand = componentData[1];
            String mouModel = componentData[2];
            String mouConnectionType = componentData[3];
            double mouPrice = Double.parseDouble(componentData[4].split(" ")[0]);
            listOfMouses.add(new Mouse("Mouse",mouBrand,mouModel,mouConnectionType,mouPrice));
            break;
          }
          case "PSU":{
            String psuBrand = componentData[1];
            String psuModel = componentData[2];
            String psuFormFactor = componentData[3];
            int psuWattage = Integer.parseInt(componentData[4]);
            double psuPrice = Double.parseDouble(componentData[5].split(" ")[0]);
            listOfPSUs.add(new PSU("PSU",psuBrand,psuModel,psuFormFactor,psuWattage,psuPrice));
            break;
          }
          default:{
            System.out.println("Invalid component specified : " + componentType);
            System.exit(-1);
            break;
          }
        }
      }
      } 
      catch (IOException e) 
      { 
        e.printStackTrace(); 
      }
  }
  public void FindPartsWithBrand(String type, String brand){
    if(type == null){
      listOfCPUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfGPUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfHarddrives
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfKeyboards
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfMemories
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfMonitors
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfMotherboards
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfMouses
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
                    listOfPSUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .forEach(System.out::println);
    }
    else{
        
        switch(type){
          case "CPU":{
            listOfCPUs
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "GPU":{
            listOfGPUs
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Hard Drive":{
            listOfHarddrives
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Keyboard":{
            listOfKeyboards
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Memory":{
            listOfMemories
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Monitor":{
            listOfMonitors
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Motherboard":{
            listOfMotherboards
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "Mouse":{
            listOfMouses
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          case "PSU":{
            listOfPSUs
              .stream()
              .filter( item -> item.getBrand().equals(brand))
              .forEach(System.out::println);
            break;
          }
          default:{
            System.out.println("Invalid component specified : " + type);
            System.exit(-1);
            break;
          }
        }
    }
  }

  public void TotalPrice(String type, String brand, String model){
    double runningSum = 0.0;
    if(type == null){
      if(brand == null){
        if(model == null){
        runningSum += listOfCPUs
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfGPUs
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfHarddrives
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfKeyboards
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfMemories
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfMonitors
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfMotherboards
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfMouses
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfPSUs
        .stream()
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        }
        else{
        runningSum += listOfCPUs
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfGPUs
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfHarddrives
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfKeyboards
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfMemories
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfMonitors
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum += listOfMotherboards
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfMouses
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum += listOfPSUs
        .stream()
        .filter( item -> item.getModel().equals(model))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        }
      }
      else{
        if(model == null){
        runningSum +=listOfCPUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfGPUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfHarddrives
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfKeyboards
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfMemories
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfMonitors
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfMotherboards
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfMouses
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfPSUs
        .stream()
        .filter( item -> item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        }
        else{
        runningSum +=listOfCPUs
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfGPUs
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfHarddrives
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfKeyboards
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfMemories
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfMonitors
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfMotherboards
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
         runningSum +=listOfMouses
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        runningSum +=listOfPSUs
        .stream()
        .filter( item -> item.getModel().equals(model) && item.getBrand().equals(brand))
        .map(x -> x.getPrice())
        .reduce(0.0,Double::sum);
        }
      }
    }
    else{
        if(brand == null){
          if(model == null){
            runningSum +=listOfCPUs
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfGPUs
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfHarddrives
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfKeyboards
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfMemories
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfMonitors
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfMotherboards
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfMouses
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
            runningSum +=listOfPSUs
            .stream()
            .filter( item -> item.getType().equals(type))
            .map(x -> x.getPrice())
            .reduce(0.0,Double::sum);
        }
        else{
          runningSum +=listOfCPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfGPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfHarddrives
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfKeyboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMemories
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMonitors
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMotherboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMouses
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfPSUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
        }
      }
      else{
        if(model == null){
          runningSum +=listOfCPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfGPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfHarddrives
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfKeyboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMemories
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMonitors
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMotherboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMouses
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfPSUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
        }
        else{
          runningSum +=listOfCPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfGPUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfHarddrives
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfKeyboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMemories
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMonitors
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
            runningSum +=listOfMotherboards
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfMouses
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
          runningSum +=listOfPSUs
          .stream()
          .filter( item -> item.getType().equals(type) && item.getModel().equals(model) && item.getBrand().equals(brand))
          .map(x -> x.getPrice())
          .reduce(0.0,Double::sum);
        }
      }
    }
    System.out.printf("%.2f USD\n", runningSum);
  }

  public void UpdateStock(){
    long updatedCounter = 0;
    updatedCounter += listOfCPUs.size();
    updatedCounter += listOfGPUs.size();
    updatedCounter += listOfHarddrives.size();
    updatedCounter += listOfKeyboards.size();
    updatedCounter += listOfMemories.size();
    updatedCounter += listOfMonitors.size();
    updatedCounter += listOfMotherboards.size();
    updatedCounter += listOfMouses.size();
    updatedCounter += listOfPSUs.size();

    List<CPU> newCPUlist = listOfCPUs
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newCPUlist.size();
    listOfCPUs = newCPUlist;
    //----------------------------------------------
    List<GPU> newGPUlist = listOfGPUs
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newGPUlist.size();
    listOfGPUs = newGPUlist;
    //----------------------------------------------
    List<HardDrive> newHarddriveList = listOfHarddrives
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newHarddriveList.size();
    listOfHarddrives = newHarddriveList;
    //----------------------------------------------
    List<Keyboard> newKeyboardList = listOfKeyboards
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newKeyboardList.size();
    listOfKeyboards = newKeyboardList;
    //----------------------------------------------
    List<Memory> newMemorylist = listOfMemories
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newMemorylist.size();
    listOfMemories = newMemorylist;
    //----------------------------------------------
    List<Monitor> newMonitorlist = listOfMonitors
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newMonitorlist.size();
    listOfMonitors = newMonitorlist;
    //----------------------------------------------
    List<Motherboard> newMotherboardsList = listOfMotherboards
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newMotherboardsList.size();
    listOfMotherboards = newMotherboardsList;
    //----------------------------------------------
    List<Mouse> newMouseList = listOfMouses
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newMouseList.size();
    listOfMouses = newMouseList;
    //----------------------------------------------
    List<PSU> newPSUList = listOfPSUs
                          .stream()
                          .filter( item -> item.getPrice() != 0.00)
                          .collect(Collectors.toList());
    updatedCounter -= newPSUList.size();
    listOfPSUs = newPSUList;
    //----------------------------------------------
    
    System.out.println(String.valueOf(updatedCounter) + " items removed.");
  }

  public void FindCheapestMemory(int capacity){
    Optional<Memory> cheapestMemory = listOfMemories
      .stream()
      .filter( x -> x.getCapacity() >= capacity)
      .sorted(Comparator.comparing(Memory::getPrice))
      .findFirst();

    System.out.println(cheapestMemory.get().toString());
  }

  public void FindFastestCPU(){
    Optional<CPU> fastestCPU = listOfCPUs
      .stream()
      .sorted(Comparator.comparing(CPU::getTotalPower))
      .reduce((first, second) -> second);

    System.out.println(fastestCPU.get().toString());
  }
}