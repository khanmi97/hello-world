

import UIKit


class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource
{

    
    var mycity:Cities =  Cities()
    
    
    @IBOutlet weak var cityTable: UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int
    {
    
        return mycity.getCount()
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell
    {
        
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath) as! CityTableViewCell
        cell.layer.borderWidth = 1.0
        
        let city = mycity.getCity(item:indexPath.row)
        
        cell.cityLbl.text = city.cityName
        cell.couLbl.text = city.countryName
    
        cell.imageLbl.image = UIImage(named: city.cityImg!)
        
        
        return cell
        
    }
    
    // delete table entry
    func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool
    {
        return true
    }
    
    func tableView(tableView: UITableView, editingStyleForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell.EditingStyle { return UITableViewCell.EditingStyle.delete }
    
    
    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath)
    {
        mycity.removeCity(item: indexPath.row)
        
        
        self.cityTable.beginUpdates()
        self.cityTable.deleteRows(at: [indexPath], with: .automatic)
        self.cityTable.endUpdates()
        
    
        
    }

    @IBAction func refreash(_ sender: AnyObject)
    {
      
        
        let alert = UIAlertController(title: "Add City", message: nil, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel, handler: nil))
        
        alert.addTextField(configurationHandler:
            { textField in
            textField.placeholder = "Enter Name of the City here"
        })
        
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler:
            { action in
            
            if let name = alert.textFields?.first?.text
            {
                print("City Name: \(name)")
                
                
                self.mycity.addCity(name: name, cou: " ", image: "city.jpg")
                
                //Method 1
               
                let indexPath = IndexPath (row: self.mycity.getCount() - 1, section: 0)
                self.cityTable.beginUpdates()
                self.cityTable.insertRows(at: [indexPath], with: .automatic)
                self.cityTable.endUpdates()
                
            }
        }))
        
        self.present(alert, animated: true)
        
        
        
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?)
    {
        let selectedIndex: IndexPath = self.cityTable.indexPath(for: sender as! UITableViewCell)!
        
        let city = mycity.getCity(item: selectedIndex.row)
        
        
        
        if(segue.identifier == "detailView")
        {
            if let viewController: DetailViewController = segue.destination as? DetailViewController {
                viewController.selectedCity = city.cityName;
            }
        }
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    


}

