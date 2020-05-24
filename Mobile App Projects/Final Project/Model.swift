//
//  Model.swift
//  cse335_Project
//
//  Created by Md Khan on 3/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import Foundation
import CoreData
import UIKit

public class Model
{
    let managedObj:NSManagedObjectContext?
    let managedObj1:NSManagedObjectContext?
    
    init (context: NSManagedObjectContext, context1: NSManagedObjectContext)
    {
        managedObj = context
        managedObj1 = context1
    }
    
    
    func saveContext(t : String, g : String, rt : Int16, rv : String)
    {
        let ent = NSEntityDescription.entity(forEntityName: "Entertainment", in: self.managedObj!)
        let entertainment = Entertainment(entity: ent!, insertInto: managedObj)
        
        entertainment.title = t
        entertainment.genre = g
        entertainment.ratings = rt
        entertainment.review = rv
        
        do
        {
            try managedObj!.save()
        }
        catch let error
        {
            print(error.localizedDescription)
        }
    }
    
    func saveContext1(ty: String, im: NSData)
    {
        let med = NSEntityDescription.entity(forEntityName: "Media", in: self.managedObj1!)
        let media = Media(entity: med!, insertInto: managedObj1)
        
        media.typeOfMedia = ty
        media.imgInfo = im
        
        do
        {
            try managedObj1?.save()
        }
        catch let error
        {
            print(error.localizedDescription)
        }
    }
}
