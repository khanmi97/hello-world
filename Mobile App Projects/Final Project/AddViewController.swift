//
//  AddViewController.swift
//  cse335_Project
//
//  Created by Md Khan on 3/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit

class AddViewController: UIViewController, UIPickerViewDelegate, UIPickerViewDataSource, UIImagePickerControllerDelegate, UINavigationControllerDelegate {
    
    
    @IBOutlet weak var titleField: UITextField!
    
    @IBOutlet weak var ratingsSeg: UISegmentedControl!
    
    @IBOutlet weak var reviewField: UITextView!
    
    
    @IBOutlet weak var chooseType: UISegmentedControl!
    
    @IBOutlet weak var genrePicker: UIPickerView!
    
    
    @IBOutlet weak var mediaImage: UIImageView!
    
    
    @IBOutlet weak var camOrLib: UISegmentedControl!
    
    // picker elements
    var genreList1 = [String]()
    var genreList2 = [String]()
    var genreList3 = [String]()
    

    
    var gn:String = ""
    var ratings:Int16 = 0
    var mediaType:String = ""
    var storeImage:NSData?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        genreList1 = ["Action", "Adventure", "Anime", "Biography", "Children", "Comedy", "Crime", "Cult", "Documentary", "Drama", "Family", "Fantasy", "Food", "Game", "History", "Horror", "Musical", "Mystery", "Reality", "Romance", "Sci-fi", "Sport", "Teen", "Thriller", "War"]
        
        genreList2 = ["Action", "Adventure", "Anime", "Biography", "Children", "Comedy", "Crime", "Cult", "Documentary", "Drama", "Family", "Fantasy", "Food", "Game", "History", "Horror", "Musical", "Mystery", "Reality", "Romance", "Sci-fi", "Sport", "Teen", "Thriller", "War"]
        
        genreList3 = ["Action", "Adventure", "Anime", "Biography", "Children", "Comedy", "Crime", "Cult", "Documentary", "Drama", "Family", "Fantasy", "Food", "Game", "History", "Horror", "Musical", "Mystery", "Reality", "Romance", "Sci-fi", "Sport", "Teen", "Thriller", "War"]
        self.genrePicker.delegate = self
        self.genrePicker.dataSource = self
    }
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 3
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        if component == 0
        {
            return genreList1.count
        }
        else if component == 1
        {
            return genreList2.count
        }
        else
        {
            return genreList3.count
        }
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        if component == 0
        {
            return genreList1[row]
        }
        else if component == 1
        {
            return genreList2[row]
        }
        else
        {
            return genreList3[row]
        }
    }
    
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        let g1 = genreList1[pickerView.selectedRow(inComponent: 0)].description
        let g2 = genreList2[pickerView.selectedRow(inComponent: 1)].description
        let g3 = genreList3[pickerView.selectedRow(inComponent: 2)].description
        
        self.gn = g1 + ", " + g2 + ", " + g3
        
    }
    
    
    @IBAction func addPic(_ sender: Any) {
        
        if self.camOrLib.selectedSegmentIndex == 0
        {
            let photoPicker = UIImagePickerController()
            photoPicker.delegate = self
            photoPicker.sourceType = .photoLibrary
            self.present(photoPicker, animated: true, completion: nil)
        }
        else
        {
            print("Sorry no camera available")
        }
        
    }
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        
        if let image = info[UIImagePickerController.InfoKey.originalImage] as? UIImage
        {
            self.mediaImage.image = image
            storeImage = image.pngData()! as NSData
        }
        
        
        picker.dismiss(animated: true, completion: nil)
    }
    
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        
        
        var title:String = self.titleField.text!
        var review:String = self.reviewField.text!
        if segue.identifier == "OpenMap"
        {

        }
        else if segue.identifier == "Browse"
        {
            let browseVC = segue.destination as! BrowseViewController
            browseVC.contentName = title
        }
        else
        {
            let mainVC = segue.destination as! ViewController
            
            switch self.ratingsSeg.selectedSegmentIndex {
            case 0:
                ratings = 1
            case 1:
                ratings = 2
            case 2:
                ratings = 3
            case 3:
                ratings = 4
            case 4:
                ratings = 5
            case 5:
                ratings = 6
            case 6:
                ratings = 7
            case 7:
                ratings = 8
            case 8:
                ratings = 9
            case 9:
                ratings = 10
            default:
                break;
            
            }
            
            switch self.chooseType.selectedSegmentIndex {
            case 0:
                mediaType = "Movie"
            case 1:
                mediaType = "TV Show"
            case 2:
                mediaType = "Game"
            default:
                break;
            }
            
            
            mainVC.m?.saveContext(t: title, g: gn, rt: ratings, rv: review)
            mainVC.m?.saveContext1(ty: mediaType, im: storeImage!)
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
