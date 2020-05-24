//
//  DetailViewController.swift
//  cse335_Project
//
//  Created by Md Khan on 4/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit
import Social

class DetailViewController: UIViewController {
    
    var selectedName:String?
    var selectedGenre:String?
    var selectedRatings:Int16?
    var selectedReview:String?
    var selectedType:String?
    var imgVar:NSData?
    
    @IBOutlet weak var reviewArea: UITextView!
    
    
    @IBOutlet weak var viewMediaImg: UIImageView!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.reviewArea.text = self.selectedType! + ": " + self.selectedName! + "\n" + "Genre: " + self.selectedGenre! + "\n" + "Ratings: " + String(self.selectedRatings!) + "\n\n" + "Review: " + self.selectedReview!
        //self.reviewArea.text = "Hello"
        
        if let picture = self.imgVar
        {
            self.viewMediaImg.image = UIImage(data: picture as Data)
        }
        else
        {
            self.viewMediaImg.image = nil
        }
        
    }
    
    
    @IBAction func shareBtn(_ sender: Any) {
        
        let alert = UIAlertController(title: "Share", message: "Share your review", preferredStyle: .actionSheet)
        let alertAction = UIAlertAction(title: "Share on Facebook", style: .default, handler: {(action) in
            if SLComposeViewController.isAvailable(forServiceType: SLServiceTypeFacebook)
            {
                let fbPost = SLComposeViewController(forServiceType: SLServiceTypeFacebook)
            fbPost!.setInitialText(self.reviewArea.text)
                
                self.present(fbPost!, animated: true, completion: nil)
            }
            else
            {
                let al = UIAlertController(title: "Error", message: "You are not connected to Facebook", preferredStyle: .alert)
                let ac = UIAlertAction(title: "OK", style: .cancel, handler: nil)
                
                al.addAction(ac)
                self.present(al, animated: true, completion: nil)
            }
        })
        alert.addAction(alertAction)
        
        self.present(alert, animated: true, completion: nil)
        
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?)
    {
        let bVc = segue.destination as? BrowseViewController
        bVc?.contentName = self.selectedName
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
