//
//  ViewController.swift
//  cse335_Project
//
//  Created by Md Khan on 3/18/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit
import CoreData

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, UISearchBarDelegate {
    
    
    @IBOutlet weak var reviewerTbl: UITableView!
    
    
    
    let managedObjectContext = (UIApplication.shared.delegate as! AppDelegate).persistentContainer.viewContext
    let managedObjectContext1 = (UIApplication.shared.delegate as! AppDelegate).persistentContainer.viewContext
    var fetchResult = [Entertainment]()
    var fetchResult1 = [Media]()
    var m:Model?
    

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        m = Model(context: managedObjectContext, context1: managedObjectContext1)
    }
    
    func fetchRecord() -> Int
    {
        let fetchRequest = NSFetchRequest<NSFetchRequestResult>(entityName: "Entertainment")
        let sort = NSSortDescriptor(key: "title", ascending: true)
        
        fetchRequest.sortDescriptors = [sort]
        fetchResult = ((try? managedObjectContext.fetch(fetchRequest)) as? [Entertainment])!
        
        return fetchResult.count
    }
    
    func fetchRecord1()
    {
        let fetchRequest = NSFetchRequest<NSFetchRequestResult>(entityName: "Media")
        fetchResult1 = ((try? managedObjectContext1.fetch(fetchRequest)) as? [Media])!
    }
    
    
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        fetchRecord1()
        
        return fetchRecord()
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell
    {
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        cell.layer.borderWidth = 1.0
        cell.textLabel?.text = fetchResult[indexPath.row].title
        
        if let picture = fetchResult1[indexPath.row].imgInfo
        {
            cell.imageView?.image = UIImage(data: picture as Data)
        }
        else
        {
            cell.imageView?.image = nil
        }
        
        return cell
    }
    
    /*func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell
    {
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath) as! MovieTableViewCell
        cell.cellLbl.text = fetchResult[indexPath.row].title
        
        return cell
    }*/
    
    func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool
    {
        return true
    }
    
    func tableView(_ tableView: UITableView, editingStyleForRowAt indexPath: IndexPath) -> UITableViewCell.EditingStyle
    {
        return UITableViewCell.EditingStyle.delete
    }
    
    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath)
    {
        if editingStyle == .delete
        {
            managedObjectContext.delete(fetchResult[indexPath.row])
            managedObjectContext1.delete(fetchResult1[indexPath.row])
            
            fetchResult.remove(at: indexPath.row)
            fetchResult1.remove(at: indexPath.row)
            
            do
            {
                try managedObjectContext.save()
            }
            catch
            {
                //error
            }
            
            do
            {
                try managedObjectContext1.save()
            }
            catch
            {
                //error
            }
            
            reviewerTbl.reloadData()
        }
    }
    
    

    @IBAction func deleteAll(_ sender: Any) {
        let fetchRequest = NSFetchRequest<NSFetchRequestResult>(entityName: "Entertainment")
        let deleteReq = NSBatchDeleteRequest(fetchRequest: fetchRequest)
        do
        {
            try managedObjectContext.execute(deleteReq)
            try managedObjectContext.save()
        }
        catch let _ as NSError
        {
            print("Error!")
        }
        
        let fetchRequest1 = NSFetchRequest<NSFetchRequestResult>(entityName: "Media")
        let deleteReq1 = NSBatchDeleteRequest(fetchRequest: fetchRequest1)
        do
        {
            try managedObjectContext1.execute(deleteReq1)
            try managedObjectContext1.save()
        }
        catch let _ as NSError
        {
            print("Error")
        }
        
        reviewerTbl.reloadData()
    }
    
    
    
    
    @IBAction func unwindToMain(_ sender: UIStoryboardSegue)
    {
        reviewerTbl.reloadData()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?)
    {
        if segue.identifier == "Detail"
        {
            let selectedIndex: IndexPath = self.reviewerTbl.indexPath(for: sender as! UITableViewCell)!
            
            let media = fetchResult[selectedIndex.row]
            let media1 = fetchResult1[selectedIndex.row]
            
            if segue.identifier == "Detail"
            {
                if let viewController : DetailViewController = segue.destination as? DetailViewController
                {
                    viewController.selectedName = media.title
                    viewController.selectedGenre = media.genre
                    viewController.selectedRatings = media.ratings
                    viewController.selectedReview = media.review
                    viewController.selectedType = media1.typeOfMedia
                    viewController.imgVar = media1.imgInfo
                }
            }
        }
    }


}

