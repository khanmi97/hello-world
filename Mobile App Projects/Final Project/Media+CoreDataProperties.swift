//
//  Media+CoreDataProperties.swift
//  cse335_Project
//
//  Created by Md Khan on 4/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//
//

import Foundation
import CoreData


extension Media {

    @nonobjc public class func fetchRequest() -> NSFetchRequest<Media> {
        return NSFetchRequest<Media>(entityName: "Media")
    }

    @NSManaged public var typeOfMedia: String?
    @NSManaged public var imgInfo: NSData?

}
