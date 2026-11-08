//
//  DOHeaderCell.h
//  Dopamine
//
//  Created by tomt000 on 26/01/2024.
//

#import <UIKit/UIKit.h>
#import <TargetConditionals.h>
#if !TARGET_OS_TV
#import <Preferences/PSTableCell.h>
#import <Preferences/PSSpecifier.h>

NS_ASSUME_NONNULL_BEGIN

@interface DOHeaderCell : UITableViewCell

@end

NS_ASSUME_NONNULL_END
#endif
