

import UIKit
import MapKit

class DetailViewController: UIViewController {
    
    var selectedCity:String?
    
    
    @IBOutlet weak var ctLbl: UILabel!
    
    @IBOutlet weak var ctImage: UIImageView!
    
    
    @IBOutlet weak var longField: UITextField!
    
    
    @IBOutlet weak var latField: UITextField!
    
    
    @IBOutlet weak var map: MKMapView!
    
    
    @IBOutlet weak var searchField: UITextField!
    
    
    @IBOutlet weak var mapView: UISegmentedControl!
    
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // geocoding
        if selectedCity == "London"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "London.jpg")
            
        }
        else if selectedCity == "Paris"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Paris.jpg")
            
            
        }
        else if selectedCity == "Sydney"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Sydney.jpg")
            
            
        }
        else if selectedCity == "Tokyo"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Tokyo.jpg")
           
            
        }
        else if selectedCity == "Cairo"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Cairo.jpg")
            
            
        }
        else
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "city.jpg")
        }
        
        let geoCoder = CLGeocoder();
        
        CLGeocoder().geocodeAddressString(selectedCity!, completionHandler:
            {(placemarks, error) in
                if placemarks!.count > 0
                {
                    let placemark = placemarks![0]
                    let location = placemark.location
                    let coord = location!.coordinate
                    let lat = String(location!.coordinate.latitude)
                    let lon = String(location!.coordinate.longitude)
                    self.longField.text = lon
                    self.latField.text = lat
                    
                    let spanning = MKCoordinateSpan.init(latitudeDelta: 0.05, longitudeDelta: 0.05)
                    let region = MKCoordinateRegion(center: placemark.location!.coordinate, span: spanning)
                    
                    self.map.setRegion(region, animated: true)
                    
                    let ani = MKPointAnnotation()
                    ani.coordinate = placemark.location!.coordinate
                    ani.title = placemark.locality
                    ani.subtitle = placemark.subLocality
                    
                    self.map.addAnnotation(ani)
                    
                }
        })
        
    }
    
    
    @IBAction func search(_ sender: Any) {
        let request = MKLocalSearch.Request()
        request.naturalLanguageQuery = self.searchField.text
        request.region = self.map.region;
        
        let sr = MKLocalSearch(request: request)
        
        sr.start(completionHandler: {(response, error) in
            if error != nil
            {
                print("Error!")
            }
            else if response!.mapItems.count == 0
            {
                print("Not Found!")
            }
            else
            {
                for i in response!.mapItems
                {
                    let annot = MKPointAnnotation()
                    annot.coordinate = i.placemark.coordinate
                    annot.title = i.name
                    
                    self.map.addAnnotation(annot)
                }
            }
        })
        
    }
    
    
    
    @IBAction func selectMapView(_ sender: UISegmentedControl) {
        
        if sender.selectedSegmentIndex == 0
        {
            self.map.mapType = .standard
        }
        else if sender.selectedSegmentIndex == 1
        {
            self.map.mapType = .satellite
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
