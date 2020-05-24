//
//  MapViewController.swift
//  cse335_Project
//
//  Created by Md Khan on 4/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit
import MapKit

class MapViewController: UIViewController {
    
    
    @IBOutlet weak var mapView: MKMapView!
    
    
    @IBOutlet weak var cityField: UITextField!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        /*let request = MKLocalSearch.Request()
        request.naturalLanguageQuery = "Movie Theater"
        request.region = self.mapView.region
        
        let search = MKLocalSearch(request: request)
        search.start(completionHandler: {(response, error) in
            if error != nil
            {
                print("Error")
            }
            else if response!.mapItems.count == 0
            {
                print("Not found")
            }
            else
            {
                for i in response!.mapItems
                {
                    let annot = MKPointAnnotation()
                    annot.coordinate = i.placemark.coordinate
                    annot.title = i.name
                    
                self.mapView.addAnnotation(annot)
                }
                
            }
        })*/
    }
    
    
    @IBAction func searchBtn(_ sender: Any) {
        CLGeocoder().geocodeAddressString(self.cityField.text!, completionHandler:
            {(placemarks, error) in
                if placemarks!.count > 0
                {
                    let placemark = placemarks![0]
                    let location = placemark.location
                    let coord = location!.coordinate
                    let lat = String(location!.coordinate.latitude)
                    let lon = String(location!.coordinate.longitude)

                    
                    let spanning = MKCoordinateSpan.init(latitudeDelta: 0.05, longitudeDelta: 0.05)
                    let region = MKCoordinateRegion(center: placemark.location!.coordinate, span: spanning)
                    
                    self.mapView.setRegion(region, animated: true)
                    
                    let ani = MKPointAnnotation()
                    ani.coordinate = placemark.location!.coordinate
                    ani.title = placemark.locality
                    ani.subtitle = placemark.subLocality
                    
                    self.mapView.addAnnotation(ani)
                    
                }
        })
        
        let alert = UIAlertController(title: "Press Find to search theaters", message: nil, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel, handler: nil))
        alert.addAction(UIAlertAction(title: "Find", style: .default, handler: {action in
                let request = MKLocalSearch.Request()
                request.naturalLanguageQuery = "Movie Theater"
                request.region = self.mapView.region
                
                let search = MKLocalSearch(request: request)
                search.start(completionHandler: {(response, error) in
                    if error != nil
                    {
                        print("Error")
                    }
                    else if response!.mapItems.count == 0
                    {
                        print("Not found")
                    }
                    else
                    {
                        for i in response!.mapItems
                        {
                            let annot = MKPointAnnotation()
                            annot.coordinate = i.placemark.coordinate
                            annot.title = i.name
                            
                            self.mapView.addAnnotation(annot)
                        }
                        
                    }
                })
            
        }))
        
        self.present(alert, animated: true)
        
    }
    
    @IBAction func mapSeg(_ sender: UISegmentedControl) {
        if sender.selectedSegmentIndex == 0
        {
            self.mapView.mapType = .standard
        }
        else if sender.selectedSegmentIndex == 1
        {
            self.mapView.mapType = .satellite
        }
        
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
