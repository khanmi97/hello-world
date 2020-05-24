

import Foundation

/*
 Model
 */

class Cities
{
    var Cities:[City] = []
    
    init()
    {
        let c1 = City(cN: "London", coN: "England", cI: "London.jpg")
        let c2 = City(cN: "Paris", coN: "France", cI: "Paris.jpg")
        let c3 = City(cN: "Sydney", coN: "Australia", cI: "Sydney.jpg")
        let c4 = City(cN: "Tokyo", coN: "Japan", cI: "Tokyo.jpg")
        let c5 = City(cN: "Cairo", coN: "Egypt", cI: "Cairo.jpg")
        
        Cities.append(c1)
        Cities.append(c2)
        Cities.append(c3)
        Cities.append(c4)
        Cities.append(c5)
        
    }
    
    func getCount() -> Int
    {
        return Cities.count
    }
    
    func getCity(item:Int) -> City
    {
        
        return Cities[item]
    }
    
    func removeCity(item:Int)
    {
        
         Cities.remove(at: item)
    }
    
    func addCity(name:String, cou: String, image: String) -> City
    {
        let ct = City(cN: name, coN: "", cI: "city.jpg")
        Cities.append(ct)
        return ct
    }
    
}
class City
{
    var cityName:String?
    var countryName:String?
    var cityImg:String?
    
    init(cN:String, coN:String, cI:String)
    {
        cityName = cN
        countryName = coN
        cityImg = cI
        
    }
    
    
}
