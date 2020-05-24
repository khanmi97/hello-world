//
//  Entertainment+CoreDataProperties.swift
//  cse335_Project
//
//  Created by Md Khan on 3/18/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//
//

import Foundation
import CoreData


extension Entertainment {

    @nonobjc public class func fetchRequest() -> NSFetchRequest<Entertainment> {
        return NSFetchRequest<Entertainment>(entityName: "Entertainment")
    }

    @NSManaged public var title: String?
    @NSManaged public var genre: String?
    @NSManaged public var imageInfo: String?
    @NSManaged public var ratings: Int16
    @NSManaged public var review: String?

}
